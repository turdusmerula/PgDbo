namespace dbo {
namespace action {

template<class C>
BulkInsert<C>::BulkInsert(collection<C>& coll, mapping::Mapping<C>& mapping, connection& conn)
	: 	coll_(coll),
		mapping_(mapping),
		conn_(conn),
		state_(PreparingHeader),
		stmt_(conn, mapping_.tableName)
{
}

template<class C>
void BulkInsert<C>::visit()
{
	// prepare request string. We do not rely on a statement as the major part of the request is unique
	state_ = PreparingHeader ;
	C dummy ;
	dummy.persist(*this) ;
	stmt_.prepare() ;

	state_ = PreparingData ;
	int i=0 ;
	for(auto ptr : coll_)
	{
		if(ptr && ptr.loaded()==false)
		{
			ptr->persist(*this) ;
			stmt_.nextRow() ;
		}
	}
	stmt_.execute() ;

	coll_.clear() ;
}

template<class C>
template<typename V>
void BulkInsert<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case PreparingHeader:
		stmt_.addColumn(field.name()) ;
		break ;
	case PreparingData:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	}

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
void BulkInsert<C>::actRef(const mapping::RefRef<D>& field)
{
}

template<class C>
template<class D>
void BulkInsert<C>::actCollection(const mapping::CollectionRef<D>& field)
{
}

}}
