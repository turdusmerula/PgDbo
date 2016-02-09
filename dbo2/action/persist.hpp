#ifndef _DBO_ACTION_PERSIST_HPP_
#define _DBO_ACTION_PERSIST_HPP_

namespace dbo2 {
namespace action {

template<class C, class Enable=void>
struct persist
{
	template<class A> static void apply(C& obj, A& action);
};

}}

#include <dbo2/action/persist.cxx>

#endif
