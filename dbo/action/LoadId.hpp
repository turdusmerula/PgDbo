#ifndef _DBO_ACTION_LOADID_HPP_
#define _DBO_ACTION_LOADID_HPP_

namespace dbo {
namespace action {

enum class LoadIdState {
	PreparingStatement,
	ReadingResult
} ;


/**
 * Load an object id from statement current state
 */
template<class C>
class LoadId
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	LoadId(IdType& id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	void preparing() { state_ = LoadIdState::PreparingStatement ; }
	void reading() { state_ = LoadIdState::ReadingResult ; }

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;
	IdType& id_ ;

	LoadIdState state_ ;

	template <class D> friend class LoadId ;
};

}}

#endif
