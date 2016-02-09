#ifndef _DBO_DBO_HPP_
#define _DBO_DBO_HPP_

#include <dbo2/database.h>

namespace dbo2 {

template<class Action, typename V>
void field(Action& action, V& value, const std::string& name, int size=-1) ;

}

#include <dbo2/dbo.cxx>

#endif
