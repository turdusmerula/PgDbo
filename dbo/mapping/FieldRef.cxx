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

template<typename V>
const std::string& FieldRef<V>::name() const
{
	return name_ ;
}

template<typename V>
int FieldRef<V>::size() const
{
	return size_ ;
}

template<typename V>
std::string FieldRef<V>::sqlType() const
{
	return traits::sql_value_traits<V>::type(size_) ;
}

template<typename V>
const std::type_info *FieldRef<V>::type() const
{
	return &typeid(V) ;
}

}}