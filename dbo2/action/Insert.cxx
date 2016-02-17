namespace dbo2 {
namespace action {

template<class C>
Insert<C>::Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		preparing_(false),
		id_(traits::dbo_traits<C>::invalidId())
{

}

template<class C>
void Insert<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		// init prepared statement, use a dummy object to init the statement
		preparing_ = true ;
		C dummy ;
		dummy.persist(*this) ;

		stmt_.prepare() ;
		preparing_ = false ;
	}

	if(ptr_)
	{
		stmt_.reset() ;
		ptr->persist(*this) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Insert failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

		if(stmt_.hasReturning())
		{
			// inserted ok, insert result should now contain the generated id
			if(stmt_.nextRow()==false)
			{
				std::stringstream ss ;
				ss << "Insert error for '" << mapping_->tableName << "' no returning id" ;
				throw Exception(ss.str()) ;
			}

			// get returned id
			char* id ;
			stmt_.read(id) ;
			ptr_.id(boost::lexical_cast< typename traits::dbo_traits<C>::IdType>(id)) ;
		}
		else
		{
			// inserted ok, set the cached id
			ptr_.id(id_) ;
		}
	}
}

template<class C>
template<typename V>
void Insert<C>::act(const mapping::FieldRef<V>& field)
{
	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
}

template<class C>
template<typename V>
void Insert<C>::actId(V& value, const std::string& name, int size)
{
	traits::sql_value_traits<V>::bind(value, stmt_, -1) ;

	if(preparing_==false)
	{
		if(value==traits::dbo_traits<C>::invalidId())
		{
			std::stringstream ss ;
			ss << "Insert failed for '" << mapping_->tableName << "' invalid id '" << ptr_.id() << "'" ;
			throw Exception(ss.str()) ;
		}
		else
			id_ = value ;
	}
}

}}
