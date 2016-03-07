#ifndef _DBO_ACTION_INSERT_HPP_
#define _DBO_ACTION_INSERT_HPP_

namespace dbo {

namespace action {

template<class C, class P=C>
class Insert
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt) ;

	Insert(ptr<C> child, const mapping::PtrRef<P>& parent, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	ptr<C> ptr_ ;
	mapping::PtrRef<P> parent_ ;	// in case of a relation, the object owner
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;
	ActionOption opt_ ;

	enum State {
		PreparingStatement=0,
		Inserting,
		Recursing,
		ReadingId
	} ;
	State state_ ;

	// id is stored during build and is given to object only if insert succeeded
	IdType id_ ;

	// helper functions that only affect pointer if they are the same type
	template <class T, class U>
	typename std::enable_if<std::is_same<T, U>::value, bool>::type
	set_ptr(dbo::ptr<T>& left, dbo::ptr<U>& right)
	{
		left = right ;
		return true ;
	} ;

	template <class T, class U>
	typename std::enable_if<!std::is_same<T, U>::value, bool>::type
	set_ptr(dbo::ptr<T>& left, dbo::ptr<U>& right) { return false ; } ;

	template <class T, class U>
	typename std::enable_if<std::is_same<T, U>::value, bool>::type
	set_ptr(dbo::weak_ptr<T>& left, dbo::ptr<U>& right)
	{
		left = right ;
		return true ;
	} ;

	template <class T, class U>
	typename std::enable_if<!std::is_same<T, U>::value, bool>::type
	set_ptr(dbo::weak_ptr<T>& left, dbo::ptr<U>& right) { return false ; } ;

	template <class T, class U> friend class Insert ;
};

}}

#endif
