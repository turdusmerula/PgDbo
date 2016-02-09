#include <dbo2/mapping/FieldInfo.h>

#include <sstream>
#include <typeinfo>

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

std::string FieldInfo::debug()
{
	std::stringstream ss ;

	ss << "name: " << name_ << "\n" ;
	ss << "sqlType: " << sqlType_ << "\n" ;
	ss << "qualifier: " << qualifier_ << "\n" ;
	ss << "foreignKeyName: " << foreignKeyName_ << "\n" ;
	ss << "foreignKeyTable: " << foreignKeyTable_ << "\n" ;
	ss << "type: " << type_->name() << "\n" ;
	ss << "flags: " <<
		((flags_ & SurrogateId)?"SurrogateId ":"") <<
		((flags_ & NaturalId)?"NaturalId ":"") <<
		((flags_ & Mutable)?"Mutable ":"") <<
		((flags_ & NeedsQuotes)?"NeedsQuotes ":"") <<
		((flags_ & ForeignKey)?"ForeignKey ":"") <<
		((flags_ & FirstDboField)?"FirstDboField ":"") <<
		"\n" ;
	ss << "fkConstraints: " << fkConstraints_ << "\n" ;

	return ss.str() ;
}
