#ifndef _DBO_ACTION_INSERTCOLLECTION_HPP_
#define _DBO_ACTION_INSERTCOLLECTION_HPP_

namespace dbo {

namespace action {

struct InsertCollectionData {
	bool collExist_ ;

	InsertCollectionData()
		:	collExist_(false)
	{}
} ;

enum InsertCollectionState {
	PreparingStatement=0,
	Inserting
} ;

template<class C, class D>
class InsertCollection
{
public:
	using IdTypeC = typename traits::dbo_traits<C>::IdType ;
	using IdTypeD = typename traits::dbo_traits<D>::IdType ;

	InsertCollection(ptr<C>& ptr, collection<D>& collection, std::shared_ptr<mapping::Mapping<C>> mappingC, std::shared_ptr<mapping::Mapping<D>> mappingD, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class E> void actId(ptr<E>& value, const std::string& name, int size, int fkConstraints) ;

	template <class E> void actPtr(const mapping::PtrRef<E>& field) ;
	template <class E> void actWeakPtr(const mapping::WeakRef<E>& field) ;

	template<class E, typename std::enable_if<std::is_same<E, D>::value, int>::type = 0>
	void actCollection(const mapping::CollectionRef<E>& field) ;

	template<class E, typename std::enable_if<!std::is_same<E, D>::value, int>::type = 0>
	void actCollection(const mapping::CollectionRef<E>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mappingC_ ;
	std::shared_ptr<mapping::Mapping<D>> mappingD_ ;
	stmt::PreparedStatement& stmt_ ;
	ptr<C>& ptr_ ;
	collection<D>& coll_ ;

	InsertCollectionState state_ ;

	std::shared_ptr<InsertCollectionData> data_ ;

	template <class T, class U> friend class InsertCollection ;
};

}}

#endif
