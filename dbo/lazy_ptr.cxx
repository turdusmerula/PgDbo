namespace dbo {

template<class C>
lazy_ptr<C>::lazy_ptr(ptr<C> ptr, connection& conn)
	:	conn_(conn),
		ptr_(ptr)
{

}

template<class C>
C* lazy_ptr<C>::operator->() const
{
    BOOST_ASSERT( ptr_ != nullptr ) ;
	return &(ptr_->value_) ;
}

template<class C>
template<class D>
collection<D>& lazy_ptr<C>::load(collection<D>& coll)
{
	auto mappingC=conn_.getMapping<C>() ;
	auto mappingD=conn_.getMapping<D>() ;
	using IdTypeC = typename traits::dbo_traits<C>::IdType ;
	using IdTypeD = typename traits::dbo_traits<D>::IdType ;

	auto& stmt=conn_.getStatement<std::pair<ptr<C>, collection<D>>, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlSelectById) ;

	action::SqlSelectCollection<C, D> action(ptr_, coll, mappingC, mappingD, stmt) ;
	action.visit() ;

	stmt.reset() ;
	action::LoadId<C> actionId(const_cast<IdTypeC&>(ptr_.id()), mappingC, stmt) ;
	actionId.visit() ;

	if(stmt.prepared()==false)
	{
		// prepare statement
		stmt.prepare() ;
	}

	// load collection
	stmt.execute() ;
	while(stmt.nextRow())
	{
		dbo::ptr<D> ptr=dbo::make_ptr<D>() ;

		action::LoadId<D> actionId(const_cast<IdTypeD&>(ptr.id()), mappingD, stmt) ;
		actionId.visit() ;

		// TODO: load content here, it will requiere to add a join to the select

		coll.push_back(ptr) ;
	}

	return coll ;
}

}
