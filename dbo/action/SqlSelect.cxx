namespace dbo {
namespace action {

//select a.name, a.value, b.id_name, b.id_age
//    from "lSimpleTable" as a
//    left join "lComplexIdTable" as b on a.id = b.complex_id
//where a.id=2


template<class C>
SqlSelect<C>::SqlSelect(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, bool whereIdClause)
	:	mapping_(mapping),
		stmt_(stmt),
		state_(SelectColumns)
{
	data_ = std::make_shared<SqlSelectData>() ;
	data_->whereIdClause_ = whereIdClause ;
}

template<class C>
SqlSelect<C>::SqlSelect(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlSelectData> data)
	:	mapping_(mapping),
		stmt_(stmt),
		data_(data),
		state_(SelectColumns)
{
}

template<class C>
void SqlSelect<C>::visit()
{
	std::stringstream& ss=data_->sql_ ;

	ss << "select " ;

	state_ = SelectColumns ;
	data_->as_letter_push(mapping_->tableName) ;

	if(data_->whereIdClause_==false && mapping_->naturalIdFieldName.empty()==true)
	{
		// add id
		static IdType dummyid ;
		actId(dummyid, mapping_->idName(), -1) ;
	}

	// iterate through fields
	static C dummy ;
	dummy.persist(*this) ;

	ss << " from \"" << mapping_->tableName << "\" as " << data_->as_letter() ;

	state_ = IdJoinNames ;
	static typename traits::dbo_traits<C>::IdType dummyid ;
	actId(dummyid, mapping_->idName(), -1) ;

	for(auto& join : data_->joins_)
	{
		ss << " left join \"" << join.tableName_ << "\" as " << join.as_ << " on " ;

		bool first=true ;
		if(data_->id_names_.size()!=join.id_names_.size())
		{
			// should not happen, means we have a bug somewhere
			std::stringstream ss ;
			ss << "Error with join id: " << data_->id_names_.size() << " names waited, got " << join.id_names_.size() << " names" ;
			throw Exception(ss.str()) ;
		}

		for(size_t iid_name=0 ; iid_name<data_->id_names_.size() ; iid_name++)
		{
			if(first==false)
				ss << " and " ;
			ss << data_->as_letter() << ".\"" << data_->id_names_[iid_name] << "\" = " << join.as_ << ".\"" << join.id_names_[iid_name] << "\"" ;
			first = false ;
		}
	}

	if(data_->whereIdClause_)
	{
		ss  << " where " ;
		state_ = IdCondition ;
		actId(dummyid, mapping_->idName(), -1) ;
	}

	stmt_.sql(ss.str()) ;

	data_->as_letter_pop() ;
}

template<class C>
template<typename V>
void SqlSelect<C>::act(const mapping::FieldRef<V>& field)
{
	std::stringstream& ss=data_->sql_ ;

	switch(state_)
	{
	case SelectColumns:
		if(data_->params_>0)
			ss << ", " ;
		ss << data_->as_letter() << ".\"" << field.name() << "\"" ;
		data_->params_++ ;
		break ;
	case IdJoinNames:
		data_->id_names_.push_back(field.name()) ;
		break ;
	case IdJoinCondition:
		data_->joins_.back().id_names_.push_back(field.name()) ;
		break ;
	case IdCondition:
		if(data_->ids_>0)
			ss << " and " ;
		ss << data_->as_letter() << ".\"" << field.name() << "\" = ?" ;
		data_->ids_++ ;
		break ;
	}
}

template<class C>
template<typename V>
void SqlSelect<C>::actId(V& value, const std::string& name, int size)
{
	field(*this, value, name) ;
}

template<class C>
template<class D>
void SqlSelect<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType=typename traits::dbo_traits<D>::IdType;
	auto mapping=conn().template getMapping<D>() ;

	std::string id_prefix=data_->id_prefix_satck_.id_prefix() ;

	if(field.nameIsJoin())
	{
		// set new join alias
		data_->as_letter_push(mapping->tableName+"_"+field.name()) ;
	}
	else
	{
		// set id prefix for composite ids
		data_->id_prefix_satck_.id_prefix_push(field.name()) ;
	}

	// this action is C type, we need D, so we create a special one for this type
	SqlSelect<D> action(mapping, stmt_, data_) ;
	action.state_ = state_ ;
	id(action, const_cast<IdType&>(field.value().id()), data_->id_prefix_satck_.id_prefix()+mapping->idName()) ;

	if(field.nameIsJoin())
	{
		SqlSelectData::Join join ;
		join.tableName_ = mapping->tableName ;
		join.as_ = data_->as_letter() ;

		data_->joins_.push_back(join) ;

		static typename traits::dbo_traits<C>::IdType dummyid ;
		SqlSelect<C> action2(mapping_, stmt_, data_) ;
		action2.state_ = IdJoinCondition ;
		id(action2, dummyid, field.name()+"_"+mapping->idName()) ;

		data_->as_letter_pop() ;
	}
	else
		data_->id_prefix_satck_.id_prefix_pop() ;
}

template<class C>
template<class D>
void SqlSelect<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType=typename traits::dbo_traits<D>::IdType;
	auto mapping=conn().template getMapping<D>() ;

	std::string id_prefix=data_->id_prefix_satck_.id_prefix() ;

	if(field.nameIsJoin())
	{
		// set new join alias
		data_->as_letter_push(mapping->tableName+"_"+field.name()) ;
	}
	else
	{
		// set id prefix for composite ids
		data_->id_prefix_satck_.id_prefix_push(field.name()) ;
	}

	// this action is C type, we need D, so we create a special one for this type
	SqlSelect<D> action(mapping, stmt_, data_) ;
	action.state_ = state_ ;
	id(action, const_cast<IdType&>(field.value().id()), data_->id_prefix_satck_.id_prefix()+mapping->idName()) ;

	if(field.nameIsJoin())
	{
		SqlSelectData::Join join ;
		join.tableName_ = mapping->tableName ;
		join.as_ = data_->as_letter() ;

		data_->joins_.push_back(join) ;

		static typename traits::dbo_traits<C>::IdType dummyid ;
		SqlSelect<C> action2(mapping_, stmt_, data_) ;
		action2.state_ = IdJoinCondition ;
		id(action2, dummyid, field.name()+"_"+mapping->idName()) ;

		data_->as_letter_pop() ;
	}
	else
		data_->id_prefix_satck_.id_prefix_pop() ;
}

template<class C>
template<class E>
void SqlSelect<C>::actCollection(const mapping::CollectionRef<E>& field)
{

}

}}
