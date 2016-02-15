namespace dbo2 {
namespace action {

template<class C>
InitStatement<C>::InitStatement(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt)
{
}

template<class C>
void InitStatement<C>::visit()
{
	// visit all fields
	C dummy ;
	dummy.persist(*this) ;

	stmt_.prepare() ;
}

template<class C>
template<typename V>
void InitStatement<C>::act(const mapping::FieldRef<V>& field)
{
	std::cout << "**** " << field.name() << std::endl ;

	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
}

}}
