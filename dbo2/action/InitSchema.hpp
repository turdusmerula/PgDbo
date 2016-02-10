#ifndef _DBO_ACTION_INITSCHEMA_HPP_
#define _DBO_ACTION_INITSCHEMA_HPP_

#include <string>
#include <memory>

namespace dbo2 {
class database ;

namespace mapping {
class MappingInfo ;
template <class T> class FieldRef ;
template <class T> class KeyRef ;
}

namespace action {

class InitSchema
{
public:
	InitSchema(database& conn, mapping::MappingInfo& mapping) ;

	template<class C> void visit(C& obj) ;


	template<typename V> void act(const mapping::FieldRef<V>& field) ;
	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class C> void actKey(const mapping::KeyRef<C>& field) ;
	void actMapping(std::shared_ptr<mapping::MappingInfo> mapping) ;

private:
	database& conn_ ;
	mapping::MappingInfo& mapping_ ;

	bool idField_ ;
	std::string foreignKeyTable_ ;
	std::string foreignKeyName_ ;
	int fkConstraints_ ;

} ;

}}

#endif
