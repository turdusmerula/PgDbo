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

void MappingInfo::init(database& conn)
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
