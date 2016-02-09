#include <dbo2/mapping/MappingInfo.h>
#include <dbo2/Exception.h>

#include <sstream>

using namespace dbo2::mapping ;

MappingInfo::MappingInfo()
	: 	initialized_(false)
{
}

MappingInfo::~MappingInfo()
{
}

void MappingInfo::init()
{
	throw Exception("Not to be done.");
}

std::string MappingInfo::primaryKeys() const
{
	if(surrogateIdFieldName!="")
		return std::string("\"")+surrogateIdFieldName+"\"" ;
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

