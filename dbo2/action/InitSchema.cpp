#include <dbo2/action/InitSchema.hpp>

using namespace dbo2::action ;

InitSchema::InitSchema(mapping::MappingInfo& mapping)
	: 	mapping_(mapping),
		idField_(false)
{
}
