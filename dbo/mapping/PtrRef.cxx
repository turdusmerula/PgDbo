namespace dbo {
namespace mapping {

template<class C>
ptr<C> PtrRef<C>::void_ptr_ ;

template<class C>
PtrRef<C>::PtrRef()
	: 	value_(void_ptr_),
		size_(0),
		fkConstraints_(0),
		nameIsJoin_(false)
{
}

template<class C>
PtrRef<C>::PtrRef(ptr<C>& value, const std::string& name, int size, int fkConstraints)
	: 	value_(value),
		name_(name),
		size_(size),
		fkConstraints_(fkConstraints),
		nameIsJoin_(false)
{
}

template<class C>
PtrRef<C>::PtrRef(ptr<C>& value, const std::string& joinname, int fkConstraints)
	: 	value_(value),
		name_(joinname),
		size_(-1),
		fkConstraints_(fkConstraints),
		nameIsJoin_(true)
{
}

template<class C>
const std::type_info* PtrRef<C>::type() const
{
	return &typeid(typename traits::dbo_traits<C>::IdType) ;
}

template<class C>
template<class A>
void PtrRef<C>::visit(A& action, connection& conn) const
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
void PtrRef<C>::debug(int tab) const
{
	std::stringstream ss ;
	std::string stab(tab, ' ') ;
	std::string stab1(tab+1, ' ') ;

	std::cout << stab << "<PtrRef>" << std::endl ;
	std::cout << stab1 << "value_:" << value_.get() << std::endl ;
	std::cout << stab1 << "name_:" << name_ << std::endl ;
	std::cout << stab1 << "size_:" << size_ << std::endl ;
	std::cout << stab1 << "fkConstraints_:" << fkConstraints_ << std::endl ;
	std::cout << stab1 << "nameIsJoin_:" << (nameIsJoin_?"true":"false") << std::endl ;
}

}}
