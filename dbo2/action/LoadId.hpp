#ifndef _DBO_ACTION_LOADID_HPP_
#define _DBO_ACTION_LOADID_HPP_

namespace dbo2 {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

template<class C>
class LoadId
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	LoadId(IdType& id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::Statement& stmt_ ;

	// id to be loaded
	IdType& id_ ;
};

}}

#endif
