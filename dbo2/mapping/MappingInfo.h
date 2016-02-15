#ifndef _DBO_MAPPING_MAPPINGINFO_H_
#define _DBO_MAPPING_MAPPINGINFO_H_

#include <dbo2/mapping/FieldInfo.h>
#include <dbo2/mapping/SetInfo.h>
//#include <dbo2/mapping/Statement.h>

#include <map>

#include <boost/optional.hpp>

namespace dbo2 {
class connection ;
namespace stmt {
class Statement ;
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

	std::map<StatementType, stmt::Statement> statements ;

	virtual void init(connection& conn) ;
	std::string primaryKeys() const ;

	std::string debug(int tab) ;
} ;

}}

//template <>
//struct hash<dbo2::mapping::MappingInfo::StatementType>
//{
//	std::size_t operator()(const dbo2::mapping::MappingInfo::StatementType& k) const
//	{
//		return hash<size_t>()(static_cast<size_t>(k)) ;
//	}
//};


#endif
