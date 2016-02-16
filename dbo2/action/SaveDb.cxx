namespace dbo2 {
namespace action {

template<class C>
SaveDb<C>::SaveDb(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt)
{

}

template<class C>
void SaveDb<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		// init prepared statement, use a dummy object to init the statement
		C dummy ;
		dummy.persist(*this) ;

		stmt_.prepare() ;
	}

	if(ptr_)
	{
		stmt_.reset() ;
		ptr->persist(*this) ;
		stmt_.execute() ;

		if(stmt_.nextRow())
		{
			// get returned id
			// TODO
			std::cout << "row" << std::endl ;
		}
	}
}

template<class C>
template<typename V>
void SaveDb<C>::act(const mapping::FieldRef<V>& field)
{
	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
}

}}
