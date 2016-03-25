namespace dbo {
namespace action {

template<class C, class D>
UpdateRelation<C, D>::UpdateRelation(ptr<C>& ptr1, ptr<D>& ptr2,
		const mapping::CollectionRef<C>& field,
		std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2,
		stmt::PreparedStatement& stmt)
	: 	field_(field),
		ptr1_(ptr1),
		ptr2_(ptr2),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt)
{
}

template<class C, class D>
void UpdateRelation<C, D>::visit()
{
}

template<class C, class D>
template<typename V>
void UpdateRelation<C, D>::act(const mapping::FieldRef<V>& field)
{
}

template<class C, class D>
template<typename V>
void UpdateRelation<C, D>::actId(V& value, const std::string& name, int size)
{
}

template<class C, class D>
template<class E>
void UpdateRelation<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
}

template<class C, class D>
template<class E>
void UpdateRelation<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
}

template<class C, class D>
template<class E>
void UpdateRelation<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
