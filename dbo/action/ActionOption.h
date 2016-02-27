#ifndef _DBO_MAPPING_ACTIONOPTION_H_
#define _DBO_MAPPING_ACTIONOPTION_H_

namespace dbo {

const int OptNone=0x00 ;
const int OptRecursive=0x01 ;
const int OptReload=(0x01<<1) ;

class ActionOption
{
public:
	explicit ActionOption(int value) ;

	bool operator==(const ActionOption& other) ;

	int value() const {	return value_; }
private:
	int value_;
} ;

/*! \brief Combines two options.
 *
 * \ingroup dbo
 */
ActionOption operator|(ActionOption lhs, ActionOption rhs) ;

namespace opt {

const ActionOption None(OptNone) ;
const ActionOption Recursive(OptRecursive) ;
const ActionOption Reload(OptReload) ;

}

}
#endif
