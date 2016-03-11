#ifndef _DBO_ACTION_SQLUPDATERELATION_HPP_
#define _DBO_ACTION_SQLUPDATERELATION_HPP_

namespace dbo {

namespace action {

struct SqlUpdateRelationData {
	std::string tableName ;

	int params_ ;
	std::stringstream sql_ ;

	SqlUpdateRelationData()
		:	params_(0)
	{}
} ;

template<class C, class D>
class SqlUpdateRelation
{
public:
	SqlUpdateRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class E> void actId(ptr<E>& value, const std::string& name, int size, int fkConstraints) ;

	template <class E> void actPtr(const mapping::PtrRef<E>& field) ;
	template <class E> void actWeakPtr(const mapping::WeakRef<E>& field) ;

	template<class E> void actCollection(const mapping::CollectionRef<E>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	const mapping::CollectionRef<C>& field_ ;
	std::shared_ptr<mapping::Mapping<C>> mapping1_ ;
	std::shared_ptr<mapping::Mapping<D>> mapping2_ ;
	stmt::PreparedStatement& stmt_ ;

	std::shared_ptr<SqlUpdateRelationData> data_ ;

	SqlUpdateRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt, std::shared_ptr<SqlUpdateRelationData> data) ;

	template <class V, class W> friend class SqlUpdateRelation ;
};

}}

#endif
