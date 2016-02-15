namespace dbo2 {
namespace action {

template<class C>
SaveDb<C>::SaveDb(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt, int column)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		column_(column)
{

}

template<class C>
void SaveDb<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(ptr_)
	{
		stmt_.reset() ;
		ptr->persist(*this) ;
		stmt_.execute() ;
	}
}

template<class C>
template<typename V>
void SaveDb<C>::act(const mapping::FieldRef<V>& field)
{
	std::cout << "**** " << field.name() << std::endl ;

	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
	column_++ ;
}

}}
