#ifndef _DBO_MAPPING_IDPREFIX_STACK_H_
#define _DBO_MAPPING_IDPREFIX_STACK_H_

#include <string>
#include <vector>

namespace dbo {
namespace mapping {

class IdPrefixStack
{
public:
	std::string id_prefix() ;
	void id_prefix_push(std::string id) { id_prefix_satck_.push_back(id) ; }
	void id_prefix_pop() { id_prefix_satck_.pop_back() ; }
private:
	std::vector<std::string> id_prefix_satck_ ;	// composite ids prefix is built from the parents names

} ;

}}
#endif
