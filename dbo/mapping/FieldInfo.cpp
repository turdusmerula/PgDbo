#include <dbo/mapping/FieldInfo.h>

#include <sstream>
#include <typeinfo>

using namespace dbo::mapping ;

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

std::string FieldInfo::debug(int tab)
{
	std::stringstream ss ;
	std::string stab(tab, ' ') ;
	std::string stab1(tab+1, ' ') ;


	ss << stab << "<FieldInfo>" << std::endl ;
	ss << stab1 << "name: " << name_ << std::endl ;
	ss << stab1 << "sqlType: " << sqlType_ << std::endl ;
	ss << stab1 << "qualifier: " << qualifier_ << std::endl ;
	ss << stab1 << "foreignKeyName: " << foreignKeyName_ << std::endl ;
	ss << stab1 << "foreignKeyTable: " << foreignKeyTable_ << std::endl ;
	ss << stab1 << "type: " << type_->name() << std::endl ;
	ss << stab1 << "flags: " <<
		((flags_ & SurrogateId)?"SurrogateId ":"") <<
		((flags_ & NaturalId)?"NaturalId ":"") <<
		((flags_ & Mutable)?"Mutable ":"") <<
		((flags_ & NeedsQuotes)?"NeedsQuotes ":"") <<
		((flags_ & ForeignKey)?"ForeignKey ":"") <<
		((flags_ & FirstDboField)?"FirstDboField ":"")
		<< std::endl;
	ss << stab1 << "fkConstraints: " << fkConstraints_ << std::endl ;

	return ss.str() ;
}
