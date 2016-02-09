#include <dbo2/mapping/FieldInfo.h>

using namespace dbo2::mapping ;

FieldInfo::FieldInfo(const std::string& name, const std::type_info *type, const std::string& sqlType, int flags)
	: 	name_(name),
		sqlType_(sqlType),
		type_(type),
		flags_(flags)
{
}

FieldInfo::FieldInfo(const std::string& name, const std::type_info *type, const std::string& sqlType, const std::string& foreignKeyTable, const std::string& foreignKeyName, int flags, int fkConstraints)
	: 	name_(name),
		sqlType_(sqlType),
		foreignKeyName_(foreignKeyName),
		foreignKeyTable_(foreignKeyTable),
		type_(type),
		flags_(flags),
		fkConstraints_(fkConstraints)
{
}

