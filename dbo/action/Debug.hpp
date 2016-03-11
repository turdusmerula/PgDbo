#ifndef _DBO_ACTION_DEBUG_HPP_
#define _DBO_ACTION_DEBUG_HPP_

namespace dbo {

namespace action {

template<class C>
class Debug
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	Debug(ptr<C> ptr, connection& conn) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return conn_ ; } ;
private:
	connection& conn_ ;

	int tab_ ;

	bool is_ptr_ ;
	ptr<C> ptr_ ;

	template <class T> friend class Debug ;
};

}}

#endif
