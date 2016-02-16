#ifndef _DBO_MAPPING_FIELDREF_HPP_
#define _DBO_MAPPING_FIELDREF_HPP_

namespace dbo2 {
namespace mapping {

template <typename V>
class FieldRef
{
public:
	FieldRef(V& value, const std::string& name, int size) ;

	const std::string& name() const ;
	int size() const ;

	std::string sqlType() const ;
	const std::type_info *type() const ;

	V& value() const
	{
		return value_;
	}

	void setValue(const V& value) const
	{
		value_ = value;
	}

private:
	V& value_ ;
	std::string name_ ;
	int size_ ;
} ;

}}

#endif
