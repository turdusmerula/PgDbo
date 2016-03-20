namespace dbo {
namespace action {

template<class C>
SelectById<C>::SelectById(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	SelectById(ptr, ptr.id(), mapping, stmt)
{
}

template<class C>
SelectById<C>::SelectById(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	SelectById(ptr<C>(), traits::dbo_traits<C>::invalidId(), mapping, stmt)
{
}

template<class C>
SelectById<C>::SelectById(ptr<C> ptr, IdType id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		id_(id),
		state_(SelectByIdState::PreparingStatement)
{
}

template<class C>
void SelectById<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		stmt_.reset() ;

		action::SqlSelect<C> action(mapping_, stmt_, true) ;
		action.visit() ;

		state_ = SelectByIdState::PreparingStatement ;

		// init prepared statement, where clause is an id
		static IdType dummy ;
		field(*this, dummy, "") ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Select statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

	}

	if(id_==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Select failed for '" << mapping_->tableName << "': invalid id '" << id_ << "'" ;
		throw Exception(ss.str()) ;
	}

	state_ = SelectByIdState::Selecting ;
	if(ptr_)
	{
		stmt_.reset() ;

		// bind id to search for
		if(mapping_->surrogateIdFieldName!=boost::none)
			field(*this, id_, mapping_->surrogateIdFieldName.get()) ;
		else
			field(*this, id_, mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Select failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

		state_ = SelectByIdState::ReadingResult ;

		if(stmt_.nextRow())
			ptr->persist(*this) ;
		else
		{
			std::stringstream ss ;
			ss << "Object not found for '" << mapping_->tableName << "' with id=" << ptr_.id() ;
			throw Exception(ss.str()) ;
		}

		if(stmt_.nextRow())
		{
			std::stringstream ss ;
			ss << "More than one line fetched for '" << mapping_->tableName << "' with id=" << ptr_.id() ;
			throw Exception(ss.str()) ;
		}

		// all was right, set id
		ptr_.id(id_) ;

		// indicate object is now loaded
		ptr_.load() ;
	}
}

template<class C>
template<typename V>
void SelectById<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case SelectByIdState::PreparingStatement:
	case SelectByIdState::Selecting:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case SelectByIdState::ReadingResult:
		traits::sql_value_traits<V>::read(field.value(), stmt_, -1) ;
		break ;
	}
}

template<class C>
template<typename V>
void SelectById<C>::actId(V& value, const std::string& name, int size)
{
	// add id fields to statement
	field(*this, value, name) ;
}

template<class C>
template<class D>
void SelectById<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	SelectById<D> action(conn().template getMapping<D>(), stmt_) ;
	action.state_ = state_ ;

	switch(state_)
	{
	case SelectByIdState::PreparingStatement:
	case SelectByIdState::Selecting:
		id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		break ;
	case SelectByIdState::ReadingResult:
		// create an empty object
		field.value() = make_ptr<D>() ;

		// load the id
		// objects are not loaded, only the id to be able to operate a lazy loading next
		id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		break ;
	}
}

template<class C>
template <class D>
void SelectById<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	SelectById<D> action(conn().template getMapping<D>(), stmt_) ;
	action.state_ = state_ ;

	switch(state_)
	{
	case SelectByIdState::PreparingStatement:
	case SelectByIdState::Selecting:
		id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		break ;
	case SelectByIdState::ReadingResult:
		// create an empty object
		field.value() = make_ptr<D>() ;

		// load the id
		// objects are not loaded, only the id to be able to operate a lazy loading next
		id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
		break ;
	}
}

template<class C>
template<class D>
void SelectById<C>::actCollection(const mapping::CollectionRef<D>& field)
{

}

}}
