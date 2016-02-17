#ifndef _DBO_ACTION_INSERT_HPP_
#define _DBO_ACTION_INSERT_HPP_

namespace dbo2 {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

template<class C>
class Insert
{
public:
	Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;
	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

private:
	ptr<C> ptr_ ;
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::Statement& stmt_ ;

	// indicate if action is preparing statement or executing it
	bool preparing_ ;

	// id is stored during build and is given to object only if insert succeeded
	typename traits::dbo_traits<C>::IdType id_ ;
};

}}

#endif