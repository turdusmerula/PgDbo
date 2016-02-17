namespace dbo2 {
namespace action {

template<class C>
LoadDb<C>::LoadDb(ptr<C> ptr, typename traits::dbo_traits<C>::IdType id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		id_(id)
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
		// init prepared statement
		stmt_.bind(boost::lexical_cast<std::string>(ptr_.id())) ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Select statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

	}

	if(ptr_)
	{
		stmt_.reset() ;

		// bind id to search for
		stmt_.bind(boost::lexical_cast<std::string>(id_)) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Select failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

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
	traits::sql_value_traits<V>::read(field.value(), stmt_, -1) ;
}

template<class C>
template<typename V>
void LoadDb<C>::actId(V& value, const std::string& name, int size)
{
	traits::sql_value_traits<V>::read(value, stmt_, -1) ;
}

}}
