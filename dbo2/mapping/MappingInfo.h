#ifndef _DBO_MAPPING_MAPPINGINFO_H_
#define _DBO_MAPPING_MAPPINGINFO_H_

#include <dbo2/mapping/FieldInfo.h>
#include <dbo2/mapping/SetInfo.h>

#include <vector>

namespace dbo2 {
namespace mapping {

class MappingInfo
{
public:
	MappingInfo() ;
	virtual ~MappingInfo() ;

	bool initialized_ ;

	std::string tableName ;
	std::string versionFieldName ;
	std::string surrogateIdFieldName ;

	std::string idCondition ;

	std::vector<FieldInfo> fields ;
	std::vector<SetInfo> sets ;

	std::vector<std::string> statements ;

	virtual void init() ;
	std::string primaryKeys() const ;
} ;

}}

#endif
