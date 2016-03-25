namespace dbo {
namespace action {

template<class C, class D>
SqlUpdateRelation<C, D>::SqlUpdateRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt)
	: 	field_(field),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt)
{
	data_ = std::make_shared<SqlUpdateRelationData>() ;
}

template<class C, class D>
SqlUpdateRelation<C, D>::SqlUpdateRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt, std::shared_ptr<SqlUpdateRelationData> data)
	: 	field_(field),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt),
		data_(data)
{
}

template<class C, class D>
void SqlUpdateRelation<C, D>::visit()
{
}

template<class C, class D>
template<typename V>
void SqlUpdateRelation<C, D>::act(const mapping::FieldRef<V>& field)
{
}

template<class C, class D>
template<typename V>
void SqlUpdateRelation<C, D>::actId(V& value, const std::string& name, int size)
{
}

template<class C, class D>
template<class E>
void SqlUpdateRelation<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
}

template<class C, class D>
template<class E>
void SqlUpdateRelation<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
}

template<class C, class D>
template<class E>
void SqlUpdateRelation<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
