namespace dbo {
namespace action {

template<class C, class D>
SqlSelectCollection<C,  D>::SqlSelectCollection(ptr<C>& ptr, collection<D>& coll, std::shared_ptr<mapping::Mapping<C>> mappingC, std::shared_ptr<mapping::Mapping<D>> mappingD, stmt::PreparedStatement& stmt)
	:	mappingC_(mappingC),
		mappingD_(mappingD),
		stmt_(stmt),
		ptr_(ptr),
		coll_(coll),
		state_(SearchCollection)
{
	data_ = std::make_shared<SqlSelectCollectionData>() ;
}

template<class C, class D>
void SqlSelectCollection<C, D>::visit()
{
	ptr_->persist(*this) ;

	if(data_->collExist_==false)
	{
		std::stringstream ss ;
		ss << "Load failed: The specified collection to table " << mappingD_->tableName << " from table " << mappingC_->tableName <<  " at address " << &coll_ << " does not belong to object " << ptr_.get()  ;
		throw Exception(ss.str()) ;
	}

	stmt_.sql(data_->sql_.str()) ;
}

template<class C, class D>
template<typename E>
void SqlSelectCollection<C, D>::act(const mapping::FieldRef<E>& field)
{
	std::stringstream& ss=data_->sql_ ;

	switch(state_)
	{
	case SearchCollection:
		break ;
	case AddIdSelect:
		if(data_->params_>0)
			ss << ", " ;
		ss << "\"" << mappingD_->tableName << "_" << field.name() << "\"" ;
		data_->params_++ ;
		break ;
	case AddIdWhere:
		if(data_->binds_>0)
			ss << " and " ;
		ss << "\"" << mappingC_->tableName << "_" << field.name() << "\"=?" ;
		data_->binds_++ ;
		break ;
	}
}

template<class C, class D>
template<typename E>
void SqlSelectCollection<C, D>::actId(E& value, const std::string& name, int size)
{
	switch(state_)
	{
	case SearchCollection:
		break ;
	case AddIdSelect:
	case AddIdWhere:
		field(*this, value, name) ;
		break ;
	}
}

template<class C, class D>
template<class E>
void SqlSelectCollection<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;
	auto mapping=conn().template getMapping<E>() ;

	std::string id_prefix=data_->id_prefix_satck_.id_prefix() ;

	// set id prefix for composite ids
	if(field.nameIsJoin()==false)
		data_->id_prefix_satck_.id_prefix_push(field.name()) ;

	id(*this, const_cast<IdType&>(field.value().id()), data_->id_prefix_satck_.id_prefix()+mapping->idName()) ;

	if(field.nameIsJoin()==false)
		data_->id_prefix_satck_.id_prefix_pop() ;
}

template<class C, class D>
template<class E>
void SqlSelectCollection<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;
	auto mapping=conn().template getMapping<E>() ;

	std::string id_prefix=data_->id_prefix_satck_.id_prefix() ;

	// set id prefix for composite ids
	if(field.nameIsJoin()==false)
		data_->id_prefix_satck_.id_prefix_push(field.name()) ;

	id(*this, const_cast<IdType&>(field.value().id()), data_->id_prefix_satck_.id_prefix()+mapping->idName()) ;

	if(field.nameIsJoin()==false)
		data_->id_prefix_satck_.id_prefix_pop() ;
}

template<class C, class D>
template<class E, typename std::enable_if<std::is_same<E, D>::value, int>::type>
void SqlSelectCollection<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
	// test if we are dealing with the same field
	if(&coll_!=&field.value())
		return ;

	switch(field.type())
	{
	case OneToOne:
		// can not happen
		break ;
	case ManyToOne:
		// select list of id from the table D
		data_->sql_ << " select " ;
		state_ = AddIdSelect ;
		id(*this, const_cast<IdTypeC&>(ptr_.id()), mappingC_->idName()) ;

		data_->sql_ << " from \"" << field.joinName() << "\"" ;

		state_ = AddIdWhere ;
		data_->sql_ << " where " ;
		id(*this, const_cast<IdTypeC&>(ptr_.id()), mappingC_->idName()) ;
		break ;
	case ManyToMany:
		// select list of id from the composite table C+D
		data_->sql_ << " select " ;
		state_ = AddIdSelect ;

		// add id from collection
		IdTypeD dummyId ;
		id(*this, dummyId, mappingD_->idName()) ;

		data_->sql_ << " from \"" << field.joinName() << "\"" ;

		state_ = AddIdWhere ;
		data_->sql_ << " where " ;
		id(*this, const_cast<IdTypeC&>(ptr_.id()), mappingC_->idName()) ;

		break ;
	}

	data_->collExist_ = true ;
}

template<class C, class D>
template<class E, typename std::enable_if<!std::is_same<E, D>::value, int>::type>
void SqlSelectCollection<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
