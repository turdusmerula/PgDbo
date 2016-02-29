#ifndef _DBO_ACTION_BULKINSERT_HPP_
#define _DBO_ACTION_BULKINSERT_HPP_

#include <sstream>

namespace dbo {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
template <class T> class PtrRef ;
}

namespace action {

template<class C>
class BulkInsert
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	BulkInsert(collection<C>& coll, mapping::Mapping<C>& mapping, connection& conn) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actRef(const mapping::RefRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return conn_ ; } ;
private:
	collection<C>& coll_ ;
	mapping::Mapping<C>& mapping_ ;
	dbo::connection& conn_ ;
	stmt::BulkStatement stmt_ ;

	std::vector<std::string> columns_ ;

	enum State {
		PreparingHeader=0,
		PreparingData
	} ;
	State state_ ;

	template <class D> friend class BulkInsert ;
};

}}

#endif
