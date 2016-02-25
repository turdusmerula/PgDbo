namespace dbo2 {
namespace action {

template<class C>
BulkInsert<C>::BulkInsert(collection<C>& coll, std::shared_ptr<mapping::Mapping<C>> mapping, connection& conn)
	: 	coll_(coll),
		mapping_(mapping),
		conn_(conn),
		state_(PreparingStatement)
{
}

template<class C>
void BulkInsert<C>::visit()
{
}

template<class C>
template<typename V>
void BulkInsert<C>::act(const mapping::FieldRef<V>& field)
{
}

template<class C>
template<typename V>
void BulkInsert<C>::actId(V& value, const std::string& name, int size)
{
}

template<class C>
template<class D>
void BulkInsert<C>::actPtr(const mapping::PtrRef<D>& field)
{
}

template<class C>
template<class D>
void BulkInsert<C>::actCollection(const mapping::CollectionRef<D>& field)
{
}

}}
