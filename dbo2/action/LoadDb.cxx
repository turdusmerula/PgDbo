namespace dbo2 {
namespace action {

template<class C>
LoadDb<C>::LoadDb(ptr<C> ptr, IdType id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		id_(id),
		state_(PreparingStatement)
{
	if(id_==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Select failed for '" << mapping_->tableName << "': invalid id '" << id_ << "'" ;
		throw Exception(ss.str()) ;
	}
}

template<class C>
void LoadDb<C>::visit()
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
void LoadDb<C>::act(const mapping::FieldRef<V>& field)
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
void LoadDb<C>::actId(V& value, const std::string& name, int size)
{
	// add id fields to statement
	field(*this, value, name) ;
}

}}
