#include <dbo2/action/InitSchema.hpp>

#include <dbo2/mapping/Field.h>
#include <dbo2/mapping/FieldInfo.h>
#include <dbo2/mapping/MappingInfo.h>

using namespace dbo2::action ;

InitSchema::InitSchema(database& conn, mapping::MappingInfo& mapping)
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
