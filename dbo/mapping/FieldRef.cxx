#include <typeinfo>

namespace dbo {
namespace mapping {

template<typename V>
FieldRef<V>::FieldRef(V& value, const std::string& name, int size)
	: 	value_(value),
		name_(name),
		size_(size)
{
}

template <typename V>
std::string FieldRef<V>::debug(int tab) const
{
	std::stringstream ss ;
	std::string stab(tab*2, ' ') ;
	std::string stab1((tab+1)*2, ' ') ;

	ss << stab << "<FieldRef>" << std::endl ;
	ss << stab1 << "name: " << name_ << std::endl ;
	ss << stab1 << "size: " << size_ << std::endl ;
	ss << stab1 << "sqlType: " << sqlType() << std::endl ;
	ss << stab1 << "type: " << type()->name() << std::endl ;
//	ss << stab1 << "value: " << value_ << std::endl ;

	return ss.str() ;
}

}}
