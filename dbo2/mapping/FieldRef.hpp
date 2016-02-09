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

	const V& value() const
	{
		return value_;
	}

	void setValue(const V& value) const
	{
		value_ = value;
	}

//	void bindValue(SqlStatement *statement, int column) const ;
//	void setValue(Session& session, SqlStatement *statement, int column) const ;

private:
	V& value_ ;
	std::string name_ ;
	int size_ ;
} ;

}}

#include <dbo2/mapping/FieldRef.cxx>

#endif
