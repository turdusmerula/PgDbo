namespace dbo2 {
namespace action {

template<class C>
SelectById<C>::SelectById(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	SelectById(ptr, traits::dbo_traits<C>::invalidId(), mapping, stmt)
{
}

template<class C>
SelectById<C>::SelectById(ptr<C> ptr, IdType id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		id_(id),
		state_(PreparingStatement)
{
}

template<class C>
void SelectById<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		state_ = PreparingStatement ;

		// init prepared statement, where clause is an id
		field(*this, const_cast<IdType&>(ptr_.id()), "") ;

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

	state_ = Selecting ;
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

		state_ = ReadingResult ;

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
	}
}

template<class C>
template<typename V>
void SelectById<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case PreparingStatement:
	case Selecting:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case ReadingResult:
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
	SelectById<D> action(field.value(), conn().template getMapping<D>(), stmt_) ;
	action.state_ = static_cast<typename SelectById<D>::State>(state_) ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

//	IdType& loadid=const_cast<IdType&>(field.value().id()) ;
//	LoadId<D> action2(loadid, conn().template getMapping<D>(), stmt_) ;
//	id(action2, loadid, field.name()) ;
//	std::cout << "id: " << loadid << std::endl ;
}

}}
