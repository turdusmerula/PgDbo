namespace dbo2 {
namespace action {

template<class C>
Update<C>::Update(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		preparing_(false)
{
	if(ptr_.id()==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Update failed for '" << mapping_->tableName << "': invalid id '" << ptr_.id() << "'" ;
		throw Exception(ss.str()) ;
	}
}

template<class C>
void Update<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		// init prepared statement, use a dummy object to init the statement
		preparing_ = true ;
		C dummy ;
		dummy.persist(*this) ;

		// set criteria id
		stmt_.bind(boost::lexical_cast<std::string>(ptr_.id())) ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Update statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

		preparing_ = false ;
	}

	if(ptr_)
	{
		stmt_.reset() ;
		ptr->persist(*this) ;

		// set criteria id
		stmt_.bind(boost::lexical_cast<std::string>(ptr_.id())) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Update failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
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
	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
}

template<class C>
template<typename V>
void Update<C>::actId(V& value, const std::string& name, int size)
{
	traits::sql_value_traits<V>::bind(value, stmt_, -1) ;
}

}}
