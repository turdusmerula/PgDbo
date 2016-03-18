namespace dbo {
namespace action {

template<class C>
LoadDb<C>::LoadDb(ptr<C>& ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt),
		ptr_(ptr)
{
}

template<class C>
void LoadDb<C>::visit()
{
	// read id
	if(mapping_->surrogateIdFieldName!=boost::none)
		field(*this, const_cast<IdType&>(ptr_.id()), mapping_->surrogateIdFieldName.get()) ;
	else
		field(*this, const_cast<IdType&>(ptr_.id()), mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;

	ptr_->persist(*this) ;

	// indicate object is loaded
	ptr_.load() ;

}

template<class C>
template<typename V>
void LoadDb<C>::act(const mapping::FieldRef<V>& field)
{
	if(traits::sql_value_traits<V>::read(field.value(), stmt_, -1)==false)
		throw Exception("Load error: read out of bounds") ;
}

template<class C>
template<typename V>
void LoadDb<C>::actId(V& value, const std::string& name, int size)
{
	// add id fields to statement
	field(*this, value, name) ;
}

template<class C>
template<class D>
void LoadDb<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	LoadDb<D> action(field.value(), conn().template getMapping<D>(), stmt_) ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
}

template<class C>
template <class D>
void LoadDb<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	LoadDb<D> action(field.value(), conn().template getMapping<D>(), stmt_) ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
}

template<class C>
template<class D>
void LoadDb<C>::actCollection(const mapping::CollectionRef<D>& field)
{

}

}}
