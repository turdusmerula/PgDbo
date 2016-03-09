#include <dbo/dbo_types.h>
#include <dbo/stmt/PreparedStatement.h>
#include <dbo/action/SqlSelectById.hpp>

using namespace dbo::action ;

std::string SqlSelectByIdData::id_prefix()
{
	std::string res ;

	bool first=true ;
	for(auto& prefix : id_prefix_satck_)
	{
		if(first==false)
			res += "_" ;
		res += prefix ;
		first = false ;
	}

	if(res.empty()==false)
		res += "_" ;
	return res ;
}
