namespace dbo2 {
namespace action {

template<class C>
LoadId<C>::LoadId(IdType& id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt),
		id_(id)
{
}

template<class C>
void LoadId<C>::visit()
{
	std::cout << "LoadId<C>::visit" << std::endl ;
}

template<class C>
template<typename V>
void LoadId<C>::act(const mapping::FieldRef<V>& field)
{
	std::cout << "LoadId<C>::act" << std::endl ;
}

template<class C>
template<typename V>
void LoadId<C>::actId(V& value, const std::string& name, int size)
{
	std::cout << "LoadId<C>::actId " << name << "=" << value << std::endl ;
	id_ = value ;
}

template<class C>
template<class D>
void LoadId<C>::actPtr(const mapping::PtrRef<D>& field)
{
	std::cout << "LoadId<C>::actPtr" << std::endl ;
}

}}
