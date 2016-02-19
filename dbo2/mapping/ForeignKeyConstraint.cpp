#include <dbo2/mapping/ForeignKeyConstraint.h>

using namespace dbo2 ;

ForeignKeyConstraint::ForeignKeyConstraint(int value)
	: value_(value)
{
}

ForeignKeyConstraint operator|(ForeignKeyConstraint lhs, ForeignKeyConstraint rhs)
{
	return ForeignKeyConstraint(lhs.value() | rhs.value()) ;
}
