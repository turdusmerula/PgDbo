#ifndef _DBO_MAPPING_FIELDREF_HPP_
#define _DBO_MAPPING_FIELDREF_HPP_

namespace dbo {
namespace mapping {

template <typename V>
class FieldRef
{
public:
	FieldRef(V& value, const std::string& name, int size) ;

	const std::string& name() const { return name_ ; }
	int size() const { return size_ ; }
	std::string sqlType() const { return traits::sql_value_traits<V>::type(size_) ; }
	const std::type_info *type() const { return &typeid(V) ; }

	V& value() const { return value_ ; }
	void setValue(const V& value) const { value_ = value ; }

	std::string debug(int tab=0) const ;
private:
	V& value_ ;
	std::string name_ ;
	int size_ ;
} ;

}}

#endif
