namespace dbo {

template<class C>
lazy_ptr<C>::lazy_ptr(ptr<C>& ptr, connection& conn)
	:	conn_(conn),
		ptr_(ptr)
{

}

template<class C>
C* lazy_ptr<C>::operator->()
{
	if(ptr_.id()==ptr_.invalidId)
	{
		auto mappingC=conn_.getMapping<C>() ;
		std::stringstream ss ;
		ss << "Lazy load failed for '" << mappingC->tableName << "': invalid id" ;
		throw Exception(ss.str()) ;
	}
	if(ptr_==nullptr)
	{
		typename traits::dbo_traits<C>::IdType id=ptr_.id() ;
		ptr_ = make_ptr<C>() ;	// make_ptr overvrites the cached id
		ptr_.id(id) ;
	}
	if(ptr_.loaded()==false)
    	conn_.load<C>(ptr_) ;
	return ptr_.get() ;
}

/*
 * TODO: this way of loading a collection member is not straightforward, should find a better syntax
 */
template<class C>
template<class D>
collection<D>& lazy_ptr<C>::load(collection<D>& coll)
{
	auto mappingC=conn_.getMapping<C>() ;
	auto mappingD=conn_.getMapping<D>() ;
	using IdTypeC = typename traits::dbo_traits<C>::IdType ;
	using IdTypeD = typename traits::dbo_traits<D>::IdType ;

	auto& stmt=conn_.getStatement<std::pair<ptr<C>, collection<D>>, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlSelectById) ;

	if(stmt.prepared()==false)
	{
		action::SqlSelectCollection<C, D> action(ptr_, coll, mappingC, mappingD, stmt) ;
		action.visit() ;

		action::LoadId<C> actionId(const_cast<IdTypeC&>(ptr_.id()), mappingC, stmt) ;
		actionId.preparing() ;
		actionId.visit() ;
	}
	else
	{
		stmt.reset() ;
		action::LoadId<C> actionId(const_cast<IdTypeC&>(ptr_.id()), mappingC, stmt) ;
		actionId.preparing() ;
		actionId.visit() ;
	}

	// prepare statement
	if(stmt.prepared()==false)
		stmt.prepare() ;

	// load collection
	coll.clear();
	stmt.execute() ;
	while(stmt.nextRow())
	{
//		dbo::ptr<D> ptr=dbo::make_ptr<D>() ;
		dbo::ptr<D> ptr ;

		action::LoadId<D> actionId(const_cast<IdTypeD&>(ptr.id()), mappingD, stmt) ;
		actionId.reading() ;
		actionId.visit() ;

		// TODO: load content here, it will requiere to add a join to the select

		coll.push_back(ptr) ;
	}

	return coll ;
}

template<class C>
ptr<C>& lazy_ptr<C>::load()
{
	if(ptr_.id()==ptr_.invalidId)
	{
		auto mappingC=conn_.getMapping<C>() ;
		std::stringstream ss ;
		ss << "Lazy load failed for '" << mappingC->tableName << "': invalid id" ;
		throw Exception(ss.str()) ;
	}
	if(ptr_==nullptr)
	{
		typename traits::dbo_traits<C>::IdType id=ptr_.id() ;
		ptr_ = make_ptr<C>() ; // make_ptr overvrites the cached id
		ptr_.id(id) ;
	}
	if(ptr_.loaded()==false)
    	conn_.load<C>(ptr_) ;
    return ptr_ ;
}

template<class C>
template<class D>
collection<D>& lazy_ptr<C>::insert(collection<D>& coll)
{
	auto mappingC=conn_.getMapping<C>() ;
	auto mappingD=conn_.getMapping<D>() ;
	using IdTypeC = typename traits::dbo_traits<C>::IdType ;
	using IdTypeD = typename traits::dbo_traits<D>::IdType ;

	auto& stmt=conn_.getStatement<std::pair<ptr<C>, collection<D>>, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;

	action::InsertCollection<C, D> action(ptr_, coll, mappingC, mappingD, stmt) ;
	action.visit() ;

	return coll ;
}

template<class C>
bool lazy_ptr<C>::loaded() const
{
	return ptr_.loaded() ;
}

template<class C>
void lazy_ptr<C>::modify(bool value)
{
	load() ;
	ptr_.modify(value) ;
}

template<class C>
bool lazy_ptr<C>::modified() const
{
	return ptr_.modified() ;
}

}
