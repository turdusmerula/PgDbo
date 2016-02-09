#include <dbo2/traits/dbo_traits.hpp>
#include <dbo2/action/persist.hpp>

#include <typeinfo>

namespace dbo2 {
namespace action {

template<class C>
void InitSchema::visit(C& obj)
{
	mapping_.surrogateIdFieldName = traits::dbo_traits<C>::surrogateIdField() ;

	persist<C>::apply(obj, *this) ;
}

template<typename V>
void InitSchema::act(const mapping::FieldRef<V>& field)
{
	int flags=mapping::FieldInfo::Mutable | mapping::FieldInfo::NeedsQuotes ;

	if(idField_)
		flags |= mapping::FieldInfo::NaturalId ; // Natural id

	if(!foreignKeyName_.empty())
		// Foreign key
		mapping_.fields.push_back(mapping::FieldInfo(field.name(), &typeid(V), field.sqlType(), foreignKeyTable_, foreignKeyName_, flags|mapping::FieldInfo::ForeignKey, fkConstraints_)) ;
	else
		// Normal field
		mapping_.fields.push_back(mapping::FieldInfo(field.name(), &typeid(V), field.sqlType(), flags)) ;
}

}}
