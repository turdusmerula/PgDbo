#ifndef _DBO_MAPPING_JOINID_H_
#define _DBO_MAPPING_JOINID_H_

#include <string>

namespace dbo2 {
namespace mapping {

struct JoinId
{
	std::string joinIdName ;
	std::string tableIdName ;
	std::string sqlType ;

	JoinId(const std::string& aJoinIdName, const std::string& aTableIdName, const std::string& aSqlType) ;
} ;

}}

#endif
