#include <dbo2/mapping/MappingInfo.h>
#include <dbo2/Exception.h>
#include <dbo2/stmt/Statement.h>

#include <sstream>

using namespace dbo2::mapping ;

MappingInfo::MappingInfo()
	: 	initialized_(false),
		naturalIdFieldSize(0)
{
}

MappingInfo::~MappingInfo()
{
}

void MappingInfo::init(connection& conn)
{
	throw Exception("Not to be done.");
}

std::string MappingInfo::primaryKeys() const
{
	if(surrogateIdFieldName!=boost::none)
		return std::string("\"")+surrogateIdFieldName.get()+"\"" ;
	else
	{
		std::stringstream result ;

		bool firstField=true ;
		for(auto& field : fields)
		{
			if(field.isIdField())
			{
				if(!firstField)
					result<<", " ;
				result << "\"" << field.name() << "\"" ;
				firstField = false ;
			}
		}

		return result.str() ;
	}
}

std::string MappingInfo::debug(int tab)
{
	std::stringstream ss ;
	std::string stab(tab, ' ') ;
	std::string stab1(tab+1, ' ') ;
	std::string stab2(tab+2, ' ') ;

	ss << stab << "<MappingInfo>" << std::endl ;
	ss << stab1 << "initialized: " << initialized_ << std::endl ;
	ss << stab1 << "tableName: " << tableName << std::endl ;
	if(surrogateIdFieldName==boost::none)
		ss << stab1 << "surrogateIdFieldName is none" << std::endl ;
	else
		ss << stab1 << "surrogateIdFieldName: " << surrogateIdFieldName.get() << std::endl ;
	ss << stab1 << "naturalIdFieldName: " << naturalIdFieldName << std::endl ;
	ss << stab1 << "naturalIdFieldSize: " << naturalIdFieldSize << std::endl ;
	ss << stab1 << "idCondition: " << idCondition << std::endl ;

	ss << stab1 << "fields: " << std::endl ;
	for(auto& field : fields)
		ss << field.debug(tab+2) ;

	ss << stab1 << "sets: " << std::endl ;
	for(auto& set : sets)
		ss << set.debug(tab+2) ;

	ss << stab1 << "statements: " << std::endl ;
	for(auto& stmt : statements)
		ss << stab2 << stmt.second.sql() << std::endl ;

	return ss.str() ;
}

