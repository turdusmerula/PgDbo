namespace dbo {
namespace action {

template<class C>
Update<C>::Update(ptr<C>& ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		state_(UpdateState::PreparingStatement),
		opt_(opt)
{
}

template<class C>
void Update<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		stmt_.reset() ;

		action::SqlUpdate<C> action(mapping_, stmt_) ;
		action.visit() ;

		state_ = UpdateState::PreparingStatement ;

		// init prepared statement, use a dummy object to init the statement
		C dummy ;
		dummy.persist(*this) ;

		// set criteria id
		field(*this, const_cast<IdType&>(ptr_.id()), "") ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Update statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}
	}

	if(ptr_.id()==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Update failed for '" << mapping_->tableName << "': invalid id '" << ptr_.id() << "'" ;
		throw Exception(ss.str()) ;
	}
	if(ptr_ && ptr_.loaded())
	{
		if(ptr_.modified() || opt_==opt::ForceUpdate)
		{
			state_ = UpdateState::Updating ;

			stmt_.reset() ;

			id_ = ptr_.id() ;
			ptr->persist(*this) ;

			// bind id to search for
			if(mapping_->surrogateIdFieldName!=boost::none)
				field(*this, const_cast<IdType&>(ptr_.id()), mapping_->surrogateIdFieldName.get()) ;
			else
				field(*this, const_cast<IdType&>(ptr_.id()), mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;

			try {
				stmt_.execute() ;
			} catch(std::exception& e) {
				std::stringstream ss ;
				ss << "Update failed for '" << mapping_->tableName << "': " << e.what() ;
				throw Exception(ss.str()) ;
			}

			// set changed id
			ptr_.id(id_) ;

			// object successfully modified
			ptr_.modify(false) ;
		}

		if(opt_==opt::Recursive)
		{
			state_ = UpdateState::Recursing ;
			ptr->persist(*this) ;
		}
	}
	else
	{
		std::stringstream ss ;
		ss << "Update failed for '" << mapping_->tableName << "': null object" ;
		throw Exception(ss.str()) ;
	}
}

template<class C>
template<typename V>
void Update<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case UpdateState::PreparingStatement:
	case UpdateState::Updating:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case UpdateState::Recursing:
		break ;
	} ;
}

template<class C>
template<typename V>
void Update<C>::actId(V& value, const std::string& name, int size)
{
	switch(state_)
	{
	case UpdateState::PreparingStatement:
	case UpdateState::Updating:
		// add id fields to statement
		field(*this, value, name) ;
		id_ = value ;
		break ;
	case UpdateState::Recursing:
		field(*this, value, name) ;
		break ;
	} ;
}

template<class C>
template<class D>
void Update<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	Update<D> action(field.value(), conn().template getMapping<D>(), stmt_, opt_) ;
	action.state_ = state_ ;

	switch(state_)
	{
	case UpdateState::PreparingStatement:
		if(field.nameIsJoin()==false)
		{
			// set the id
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		}
		break ;
	case UpdateState::Updating:
		if(field.value().id()==field.value().invalidId && (field.fkConstraints() & FKNotNull))
		{
			std::stringstream ss ;
			ss << "Update failed for '" << mapping_->tableName << "': FKNotNull constraint failed for '" << field.name() << "'" ;
			throw Exception(ss.str()) ;
		}

		if(field.nameIsJoin()==false)
		{
			// set the id
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		}

		break ;
	case UpdateState::Recursing:
		if(opt_==opt::Recursive && field.value().loaded())
		{
			auto mapping=conn().template getMapping<D>() ;
			auto& stmt=conn().template getStatement<D, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlUpdate) ;

			// update by giving ptr_ as a parent
			action::Update<D> action(field.value(), mapping, stmt, opt_) ;
			action.visit() ;
		}
		break ;
	}
}

template<class C>
template <class D>
void Update<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	dbo::ptr<D> ptr ;
	ptr.id(field.value().id()) ;
	if(field.value().expired()==false)
		ptr = field.value().lock() ;

	// this action is C type, we need D, so we create a special one for this type
	Update<D> action(ptr, conn().template getMapping<D>(), stmt_, opt_) ;
	action.state_ = state_ ;

	switch(state_)
	{
	case UpdateState::PreparingStatement:
	case UpdateState::Updating:
		if(field.nameIsJoin()==false)
		{
			// load the id
			// objects are not loaded, only the id to be able to operate a lazy loading next
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		}
		break ;
	case UpdateState::Recursing:
		// weak ptr are endpoints, we do not recurse inside
		break ;
	} ;
}

template<class C>
template<class D>
void Update<C>::actCollection(const mapping::CollectionRef<D>& field)
{
}


}}
