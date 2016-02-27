#include <dbo/mapping/JoinId.h>

namespace dbo {
namespace mapping {

JoinId::JoinId(const std::string& aJoinIdName, const std::string& aTableIdName, const std::string& aSqlType)
	: 	joinIdName(aJoinIdName),
		tableIdName(aTableIdName),
		sqlType(aSqlType)
{
}

}}
