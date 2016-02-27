#include "dbo/action/ActionOption.h"

using namespace dbo ;

ActionOption::ActionOption(int value)
	:	value_(value)
{}

bool ActionOption::operator==(const ActionOption& other)
{
	return value_ & other.value_ ;
}

ActionOption operator|(ActionOption lhs, ActionOption rhs)
{
	return ActionOption(lhs.value() | rhs.value()) ;
}

