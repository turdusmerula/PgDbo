#include <dbo/mapping/IdPrefixStack.h>

using namespace dbo::mapping ;

std::string IdPrefixStack::id_prefix()
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
