#ifndef _DBO_ACTION_DELETE_HPP_
#define _DBO_ACTION_DELETE_HPP_

namespace dbo {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

template<class C>
class Delete
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	Delete(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;
	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	ptr<C> ptr_ ;
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;

	enum State {
		PreparingStatement,
		Deleting,
	} ;
	State state_ ;

	template <class D> friend class Delete ;
};

}}

#endif
