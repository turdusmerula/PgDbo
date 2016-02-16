namespace dbo2 {
namespace action {

template<class C>
LoadDb<C>::LoadDb(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt)
{

}

template<class C>
void LoadDb<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		// init prepared statement
		stmt_.bind(boost::lexical_cast<std::string>(ptr_.id())) ;
		stmt_.prepare() ;
	}

	if(ptr_)
	{
		stmt_.reset() ;

		stmt_.bind(boost::lexical_cast<std::string>(ptr_.id())) ;
		stmt_.execute() ;

		if(stmt_.nextRow())
			ptr->persist(*this) ;
		else
		{
			std::stringstream ss ;
			ss << "Object not found for '" << mapping_->tableName << "' with id=" << ptr_.id() ;
			throw Exception(ss.str()) ;
		}
		// TODO set id value

		if(stmt_.nextRow())
		{
			std::stringstream ss ;
			ss << "More than one line fetched '" << mapping_->tableName << "' with id=" << ptr_.id() ;
			throw Exception(ss.str()) ;
		}

	}
}

template<class C>
template<typename V>
void LoadDb<C>::act(const mapping::FieldRef<V>& field)
{
	traits::sql_value_traits<V>::read(field.value(), stmt_, -1) ;
}

}}
