#ifndef _DBO_ACTION_INSERT_HPP_
#define _DBO_ACTION_INSERT_HPP_

namespace dbo {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
template <class T> class PtrRef ;
}

namespace action {

template<class C>
class Insert
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	ptr<C> ptr_ ;
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;

	enum State {
		PreparingStatement=0,
		Inserting,
		ReadingId
	} ;
	State state_ ;

	// id is stored during build and is given to object only if insert succeeded
	IdType id_ ;

	template <class D> friend class Insert ;
};

}}

#endif
