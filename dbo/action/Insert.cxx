namespace dbo {
namespace action {


template<class C, class P>
Insert<C, P>::Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		state_(PreparingStatement),
		id_(traits::dbo_traits<C>::invalidId()),
		opt_(opt)
{
}

template<class C, class P>
Insert<C, P>::Insert(ptr<C> child, const mapping::PtrRef<P>& parent, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt)
	: 	ptr_(child),
		parent_(parent),
		mapping_(mapping),
		stmt_(stmt),
		state_(PreparingStatement),
		id_(traits::dbo_traits<C>::invalidId()),
		opt_(opt)
{
}

template<class C, class P>
void Insert<C, P>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		action::SqlInsert<C> action(mapping_, stmt_) ;
		action.visit() ;

        // init prepared statement, use a dummy object to init the statement
		state_ = PreparingStatement ;
		static C dummy ;
		dummy.persist(*this) ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Insert statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}
	}

	if(!ptr)
	{
		std::stringstream ss ;
		ss << "Insert error for '" << mapping_->tableName << "': passing null ptr" ;
		throw Exception(ss.str()) ;
	}
	else
	{
		if(ptr_.orphan())
		{
			state_ = Inserting ;
			stmt_.reset() ;
			ptr->persist(*this) ;

			try {
				stmt_.execute() ;
			} catch(std::exception& e) {
				std::stringstream ss ;
				ss << "Insert failed for '" << mapping_->tableName << "': " << e.what() ;
				throw Exception(ss.str()) ;
			}

			state_ = ReadingId ;
			if(stmt_.hasReturning())
			{
				// inserted ok, insert result should now contain the generated id
				if(stmt_.nextRow()==false)
				{
					std::stringstream ss ;
					ss << "Insert error for '" << mapping_->tableName << "': no returning id" ;
					throw Exception(ss.str()) ;
				}

				// read id
				if(mapping_->surrogateIdFieldName!=boost::none)
					field(*this, const_cast<IdType&>(ptr_.id()), mapping_->surrogateIdFieldName.get()) ;
				else
					field(*this, const_cast<IdType&>(ptr_.id()), mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;

				// indicate object is loaded
				ptr_.load() ;
			}
			else
			{
				// inserted ok, set the cached id
				ptr_.id(id_) ;

				// indicate object is loaded
				ptr_.load() ;
			}
		}

		if(opt_==opt::Recursive)
		{
			state_ = Recursing ;
			ptr->persist(*this) ;
		}

	}
}

template<class C, class P>
template<typename V>
void Insert<C, P>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case PreparingStatement:
	case Inserting:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case Recursing:
		break ;
	case ReadingId:
		traits::sql_value_traits<V>::read(field.value(), stmt_, -1) ;
		break ;
	}
}

template<class C, class P>
template<typename V>
void Insert<C, P>::actId(V& value, const std::string& name, int size)
{
	switch(state_)
	{
	case PreparingStatement:
		// add id fields to statement
		field(*this, value, name) ;
		break ;
	case Inserting:
		if(opt_==opt::Recursive)
		{
			// in case of a composite id, recurse id values to resolve parents
			state_ = Recursing ;
			field(*this, value, name) ;
			state_ = Inserting ;
		}

		if(value==traits::dbo_traits<C>::invalidId())
		{
			std::stringstream ss ;
			ss << "Insert failed for '" << mapping_->tableName << "', invalid id " << name << "='" << ptr_.id() << "'" ;
			throw Exception(ss.str()) ;
		}

		// in case of a statement with a natural id we see the id here, put it in cache
		id_ = value ;

		// add id fields to statement
		field(*this, value, name) ;
		break ;
	case Recursing:
		break ;
	case ReadingId:

		break ;
	}
}

template<class C, class P>
template<class D>
void Insert<C, P>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// automatic set parent if it exists
	if(parent_.value()!=nullptr && field.name()==parent_.name())
		set_ptr(field.value(), parent_.value()) ;

	if(field.nameIsJoin()==true)
	{
		// nameIsJoin indicates thet this object does not really exists but needs a join from another table
		// as we are in an insert it does nothing, the following code is only here to follow the pointer in case of a recursion mode
		switch(state_)
		{
		case PreparingStatement:
			// in case of a OneToOne relation the data is on the belongsTo side, there is nothing to insert here
			break ;
		case Inserting:
			if((field.fkConstraints()&FKNotNull) && field.value()==nullptr)
			{
				std::stringstream ss ;
				ss << "Insert failed for '" << mapping_->tableName << "': FKNotNull constraint failed for '" << field.name() << "'" ;
				throw Exception(ss.str()) ;
			}
			break ;
		case Recursing:
			if(opt_==opt::Recursive)
			{
				auto mapping=conn().template getMapping<D>() ;
				auto& stmt=conn().template getStatement<D, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;

				// insert by giving ptr_ as a parent
				action::Insert<D, C> action(field.value(), mapping::PtrRef<C>(ptr_, field.name(), field.fkConstraints()), mapping, stmt, opt_) ;
				action.visit() ;
			}
			break ;
		case ReadingId:
			break ;
		}
	}
	else
	{
		// this action is C type, we need D, so we create a special one for this type
		Insert<D> action(field.value(), conn().template getMapping<D>(), stmt_, opt_) ;
		action.state_ = static_cast<typename Insert<D>::State>(state_) ;

		switch(state_)
		{
		case PreparingStatement:
			// add id fields to statement
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
			break ;
		case Inserting:
			if(field.value().id()==traits::dbo_traits<D>::invalidId())
			{
				std::stringstream ss ;
				ss << "Insert failed for '" << mapping_->tableName << "' invalid id " << field.name() << "='" << field.value().id() << "'" ;
				throw Exception(ss.str()) ;
			}

			// add id fields to statement
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

			break ;
		case Recursing:
			if(opt_==opt::Recursive)
			{
				auto mapping=conn().template getMapping<D>() ;
				auto& stmt=conn().template getStatement<D, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;

				// insert by giving ptr_ as a parent
				action::Insert<D, C> action(field.value(), mapping::PtrRef<C>(ptr_, field.name(), field.fkConstraints()), mapping, stmt, opt_) ;
				action.visit() ;
			}
			break ;
		case ReadingId:
			break ;
		}
	}
}

template<class C, class P>
template<class D>
void Insert<C, P>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// automatic set parent if it exists
	if(parent_.value()!=nullptr && field.name()==parent_.name())
		set_ptr(field.value(), parent_.value()) ;

	// in case of a OneToOne relation nothing to do
	if(field.nameIsJoin()==true)
	{
		switch(state_)
		{
		case PreparingStatement:
			// in case of a OneToOne relation the data is on the belongsTo side
			break ;
		case Inserting:
			if((field.fkConstraints()&FKNotNull) && ( field.value().expired() || field.value().lock().orphan()))
			{
				std::stringstream ss ;
				ss << "Insert failed for '" << mapping_->tableName << "': FKNotNull constraint failed for '" << field.name() << "'" ;
				throw Exception(ss.str()) ;
			}
			break ;
		case Recursing:
			// weak ptr are endpoints, we do not recurse inside
			break ;
		case ReadingId:
			break ;
		}
	}
	else
	{
		dbo::ptr<D> ptr ;
		if(field.value().expired()==false)
			ptr = field.value().lock() ;

		// this action is C type, we need D, so we create a special one for this type
		Insert<D> action(ptr, conn().template getMapping<D>(), stmt_, opt_) ;
		action.state_ = static_cast<typename Insert<D>::State>(state_) ;

		switch(state_)
		{
		case PreparingStatement:
			// add id fields to statement
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
			break ;
		case Inserting:
			if(field.value().id()==traits::dbo_traits<D>::invalidId())
			{
				std::stringstream ss ;
				ss << "Insert failed for '" << mapping_->tableName << "' invalid id " << field.name() << "='" << field.value().id() << "'" ;
				throw Exception(ss.str()) ;
			}

			// add id fields to statement
			id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

			break ;
		case Recursing:
			// weak ptr are endpoints, we do not recurse inside
			break ;
		case ReadingId:
			break ;
		}
	}
}

template<class C, class P>
template<class D>
void Insert<C, P>::actCollection(const mapping::CollectionRef<D>& field)
{
	switch(state_)
	{
	case PreparingStatement:
		break ;
	case Inserting:
		break ;
	case Recursing:
		if(opt_==opt::Recursive)
		{
			auto mapping=conn().template getMapping<D>() ;
			auto& stmt=conn().template getStatement<D, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;

			for(auto ptr : field.value())
			{
				action::Insert<D, C> action(ptr, mapping::PtrRef<C>(ptr_, field.joinName(), field.fkConstraints()), mapping, stmt, opt_) ;
				action.visit() ;

				if(field.type()==ManyToMany)
				{
					// insert into join table
					auto& stmt=conn().template getStatement<std::pair<D, P>, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;

					InsertRelation<D, C> action(ptr, ptr_, field, mapping, mapping_, stmt) ;
					action.visit() ;
				}
			}
		}
		break ;
	case ReadingId:
		break ;
	}

}

}}
