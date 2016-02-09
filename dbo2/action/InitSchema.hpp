#ifndef _DBO_ACTION_INITSCHEMA_HPP_
#define _DBO_ACTION_INITSCHEMA_HPP_

#include <dbo2/mapping/MappingInfo.h>
#include <dbo2/mapping/FieldRef.hpp>

namespace dbo2 {
namespace action {

class InitSchema
{
public:
	InitSchema(mapping::MappingInfo& mapping) ;

	template<class C> void visit(C& obj) ;
	template<typename V> void act(const mapping::FieldRef<V>& field) ;

private:
	mapping::MappingInfo& mapping_ ;

	bool idField_ ;
	std::string foreignKeyTable_ ;
	std::string foreignKeyName_ ;
	int fkConstraints_ ;

} ;

}}

#include <dbo2/action/InitSchema.cxx>

#endif
