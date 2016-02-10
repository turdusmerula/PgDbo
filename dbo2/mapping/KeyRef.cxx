namespace dbo2 {
namespace mapping {

template<class C>
KeyRef<C>::KeyRef(key<C>& key, const std::string& name, int size, int fkConstraints)
	: 	key_(key),
		name_(name),
		size_(size),
		fkConstraints_(fkConstraints)
{
}

template<class C>
const std::type_info* KeyRef<C>::type() const
{
	return &typeid(typename traits::dbo_traits<C>::IdType) ;
}

template<class C>
template<class A>
void KeyRef<C>::visit(A& action, database& conn) const
{
	typename traits::dbo_traits<C>::IdType id ;

//	if(action.setsValue())
//		id = traits::dbo_traits<C>::invalidId() ;
//	else
//		id = key_.id() ;

	std::string idFieldName = "stub" ;
	int size = size_ ;

	std::shared_ptr<mapping::MappingInfo> mapping=conn.getMapping<C>() ;
	action.actMapping(mapping) ;
	idFieldName = mapping->naturalIdFieldName ;
	size = mapping->naturalIdFieldSize ;

	if(idFieldName.empty())
		idFieldName = mapping->surrogateIdFieldName.get() ;

	field(action, id, name_+"_"+idFieldName, size) ;

//	LoadLazyHelper<C, A>::loadLazy(value_, id, session) ;
}

}}
