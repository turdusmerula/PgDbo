#ifndef _DBO_MAPPING_MAPPINGINFO_H_
#define _DBO_MAPPING_MAPPINGINFO_H_

#include <dbo2/mapping/FieldInfo.h>
#include <dbo2/mapping/SetInfo.h>

#include <map>

#include <boost/optional.hpp>

namespace dbo2 {
class connection ;
namespace stmt {
class PreparedStatement ;
}

namespace mapping {

class MappingInfo
{
public:
	MappingInfo() ;
	virtual ~MappingInfo() ;

	enum StatementType
	{
		SqlInsert=0,
		SqlUpdate=1,
		SqlDelete=2,
		SqlSelectById=3,
		FirstSqlSelectSet=4
	} ;

	bool initialized_ ;

	std::string tableName ;
	boost::optional<std::string> surrogateIdFieldName ;

	std::string naturalIdFieldName ; // for non-auto generated id
	int naturalIdFieldSize ;         // for non-auto generated id

	std::string idCondition ;

	std::vector<FieldInfo> fields ;
	std::vector<SetInfo> sets ;

	std::map<size_t, stmt::PreparedStatement> statements ;

	virtual void init(connection& conn) ;
	std::string primaryKeys() const ;

	std::string debug(int tab) ;
} ;

}}

#endif
