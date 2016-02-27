#ifndef _DBO_ACTION_INITSCHEMA_HPP_
#define _DBO_ACTION_INITSCHEMA_HPP_

#include <string>
#include <memory>

#include <dbo/mapping/SetInfo.h>

namespace dbo {
class connection ;

namespace mapping {
class MappingInfo ;
template <class T> class CollectionRef ;
template <class T> class FieldRef ;
template <class T> class PtrRef ;
}

namespace action {

class InitSchema
{
public:
	InitSchema(connection& conn, mapping::MappingInfo& mapping) ;

	template<class C> void visit(C& obj) ;


	template<typename V> void act(const mapping::FieldRef<V>& field) ;
	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class C> void actPtr(const mapping::PtrRef<C>& field) ;
	void actMapping(std::shared_ptr<mapping::MappingInfo> mapping) ;
	template<class C> void actCollection(const mapping::CollectionRef<C>& field) ;

	connection& conn()
	{
		return conn_ ;
	}

private:
	connection& conn_ ;
	mapping::MappingInfo& mapping_ ;

	bool idField_ ;
	std::string foreignKeyTable_ ;
	std::string foreignKeyName_ ;
	int fkConstraints_ ;

	std::string createJoinName(RelationType type, const std::string& c1, const std::string& c2) ;

} ;

}}

#endif
