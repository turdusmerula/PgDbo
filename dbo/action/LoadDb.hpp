#ifndef _DBO_ACTION_LOADDB_HPP_
#define _DBO_ACTION_LOADDB_HPP_

namespace dbo {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

/**
 * Load an object from statement current state
 */
template<class C>
class LoadDb
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	LoadDb(ptr<C>& ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;
	ptr<C>& ptr_ ;

	template <class D> friend class LoadDb ;
};

}}

#endif
