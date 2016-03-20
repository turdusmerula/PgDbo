#ifndef _DBO_ACTION_SQLSELECTCOLLECTION_HPP_
#define _DBO_ACTION_SQLSELECTCOLLECTION_HPP_

#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <boost/lexical_cast.hpp>

namespace dbo {

namespace action {

struct SqlSelectCollectionData {
	int params_ ;
	int binds_ ;
	std::stringstream sql_ ;
	bool collExist_ ;

	mapping::IdPrefixStack id_prefix_satck_ ;	// composite ids prefix is built from the parents names

	SqlSelectCollectionData()
		:	collExist_(false),
			params_(0),
			binds_(0)
	{}
} ;

enum SqlSelectCollectionState {
	SearchCollection,
	AddIdSelect,
	AddIdWhere
} ;


template<class C, class D>
class SqlSelectCollection
{
public:
	using IdTypeC=typename traits::dbo_traits<C>::IdType ;
	using IdTypeD=typename traits::dbo_traits<D>::IdType ;

	SqlSelectCollection(ptr<C>& ptr, collection<D>& coll, std::shared_ptr<mapping::Mapping<C>> mappingC, std::shared_ptr<mapping::Mapping<D>> mappingD, stmt::PreparedStatement& stmt) ;

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

	std::shared_ptr<SqlSelectCollectionData> data_ ;
	SqlSelectCollectionState state_ ;

	template <class E, class F> friend class SqlSelectCollection ;
};

}}

#endif
