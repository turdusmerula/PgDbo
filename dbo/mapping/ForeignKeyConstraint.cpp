#include <dbo/mapping/ForeignKeyConstraint.h>

using namespace dbo ;

ForeignKeyConstraint::ForeignKeyConstraint(int value)
	: value_(value)
{
}

ForeignKeyConstraint dbo::operator|(ForeignKeyConstraint lhs, ForeignKeyConstraint rhs)
{
	return ForeignKeyConstraint(lhs.value() | rhs.value()) ;
}
