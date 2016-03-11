namespace dbo {
namespace mapping {

template<class C>
WeakRef<C>::WeakRef(weak_ptr<C>& value, const std::string& name, int size, int fkConstraints)
	: 	value_(value),
		name_(name),
		size_(size),
		fkConstraints_(fkConstraints),
		nameIsJoin_(false)
{
}

template<class C>
WeakRef<C>::WeakRef(weak_ptr<C>& value, const std::string& joinname, int fkConstraints)
	: 	value_(value),
		name_(joinname),
		size_(-1),
		fkConstraints_(fkConstraints),
		nameIsJoin_(true)
{
}

template<class C>
const std::type_info* WeakRef<C>::type() const
{
	return &typeid(typename traits::dbo_traits<C>::IdType) ;
}

template<class C>
template<class A>
void WeakRef<C>::visit(A& action, connection& conn) const
{
	typename traits::dbo_traits<C>::IdType id ;

	std::string idFieldName = "stub" ;
	int size = size_ ;

	std::shared_ptr<mapping::MappingInfo> mapping=conn.getMapping<C>() ;
	action.actMapping(mapping) ;
	idFieldName = mapping->naturalIdFieldName ;
	size = mapping->naturalIdFieldSize ;

	if(idFieldName.empty())
		idFieldName = mapping->surrogateIdFieldName.get() ;

	field(action, id, name_+"_"+idFieldName, size) ;
}

template<class C>
std::string WeakRef<C>::debug(int tab) const
{
	std::stringstream ss ;
	std::string stab(tab*2, ' ') ;
	std::string stab1((tab+1)*2, ' ') ;

	ss << stab << "<WeakRef>" << std::endl ;
	if(value_.expired()==false)
		ss << stab1 << "value_: " << value_.lock().get() << std::endl ;
	else
		ss << stab1 << "value_: expired" << std::endl ;
	ss << stab1 << "name_: " << name_ << std::endl ;
	ss << stab1 << "size_: " << size_ << std::endl ;
	ss << stab1 << "fkConstraints_: " << fkConstraints_ << std::endl ;
	ss << stab1 << "nameIsJoin_: " << (nameIsJoin_?"true":"false") << std::endl ;

	return ss.str() ;
}

}}
