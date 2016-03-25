#ifndef _DBO_ACTION_UPDATE_HPP_
#define _DBO_ACTION_UPDATE_HPP_

namespace dbo {

namespace action {

enum class UpdateState {
	PreparingStatement,
	Updating,
	Recursing
} ;

template<class C>
class Update
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	Update(ptr<C>& ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, ActionOption opt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	ptr<C>& ptr_ ;
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;
	ActionOption opt_ ;

	IdType id_ ;	// new id_ positionned during update

	UpdateState state_ ;

	template <class D> friend class Update ;
};

}}

#endif
