#include <dbo/action/InitSchema.hpp>

#include <dbo/mapping/Field.h>
#include <dbo/mapping/FieldInfo.h>
#include <dbo/mapping/MappingInfo.h>

using namespace dbo::action ;

InitSchema::InitSchema(connection& conn, mapping::MappingInfo& mapping)
	: 	conn_(conn),
		mapping_(mapping),
		idField_(false)
{
}

void InitSchema::actMapping(std::shared_ptr<mapping::MappingInfo> mapping)
{
	if(!mapping->initialized_)
		mapping->init(conn_) ;
}

std::string InitSchema::createJoinName(RelationType type, const std::string& c1, const std::string& c2)
{
	if(type==ManyToOne)
		return c1 ;
	else
	{
		std::string t1=c1 ;
		std::string t2=c2 ;
		if(t2<t1)
			std::swap(t1, t2) ;

		return t1+"_"+t2 ;
	}
}
