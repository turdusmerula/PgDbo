namespace dbo {
namespace action {

template<class C>
SqlUpdate<C>::SqlUpdate(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt)
{
	data_ = std::make_shared<SqlUpdateData>() ;
}

template<class C>
SqlUpdate<C>::SqlUpdate(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlUpdateData> data)
	:	stmt_(stmt),
		data_(data)
{
}

template<class C>
void SqlUpdate<C>::visit()
{
	std::stringstream& ss=data_->sql_ ;

	ss << "update \"" << mapping_->tableName << "\" set " ;

	state_ = PreparingValues ;
	static C dummy ;
	dummy.persist(*this) ;

	ss << " where " ;

	state_ = PreparingId ;
	static typename traits::dbo_traits<C>::IdType dummyid ;
	actId(dummyid, mapping_->idName(), -1) ;

	stmt_.sql(ss.str()) ;
}

template<class C>
template<typename V>
void SqlUpdate<C>::act(const mapping::FieldRef<V>& field)
{
	std::stringstream& ss=data_->sql_ ;

	switch(state_)
	{
	case PreparingValues:
		if(data_->params_>0)
			ss << ", " ;
		data_->params_++ ;
		break ;
	case PreparingId:
		if(data_->idparams_>0)
			ss << " and " ;
		data_->idparams_++ ;
		break ;
	}

	ss << "\"" << field.name() << "\" = ?" ;
}

template<class C>
template<typename V>
void SqlUpdate<C>::actId(V& value, const std::string& name, int size)
{
	field(*this, value, name) ;
}

template<class C>
template<class D>
void SqlUpdate<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;
	auto mapping=conn().template getMapping<D>() ;

	if(field.nameIsJoin()==false)
	{
		// not in a one to one connection

		// this action is C type, we need D, so we create a special one for this type
		SqlUpdate<D> action(conn().template getMapping<D>(), stmt_, data_) ;
		action.state_ = state_ ;
		id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping->idName()) ;
	}
}

template<class C>
template<class D>
void SqlUpdate<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;
	auto mapping=conn().template getMapping<D>() ;

	if(field.nameIsJoin()==false)
	{
		// not in a one to one connection

		// this action is C type, we need D, so we create a special one for this type
		SqlUpdate<D> action(conn().template getMapping<D>(), stmt_, data_) ;
		action.state_ = state_ ;
		id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping->idName()) ;
	}
}

template<class C>
template<class D>
void SqlUpdate<C>::actCollection(const mapping::CollectionRef<D>& field)
{
}

}}
