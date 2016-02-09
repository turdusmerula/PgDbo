#include <dbo2/traits/StdSqlTraits.h>

#include <typeinfo>

namespace dbo2 {
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

//template<typename V>
//void FieldRef<V>::bindValue(SqlStatement *statement, int column) const
//{
//	sql_value_traits<V>::bind(value_, statement, column, size_);
//}
//
//template<typename V>
//void FieldRef<V>::setValue(Session& session, SqlStatement *statement, int column) const
//{
//	sql_value_traits<V>::read(value_, statement, column, size_);
//}

}}
