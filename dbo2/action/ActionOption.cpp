#include "dbo2/action/ActionOption.h"

using namespace dbo2 ;

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

