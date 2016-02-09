#include <dbo2/mapping/JoinId.h>

namespace dbo2 {
namespace mapping {

JoinId::JoinId(const std::string& aJoinIdName, const std::string& aTableIdName, const std::string& aSqlType)
	: 	joinIdName(aJoinIdName),
		tableIdName(aTableIdName),
		sqlType(aSqlType)
{
}

}}
