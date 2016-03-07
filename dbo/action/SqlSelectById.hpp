#ifndef _DBO_ACTION_SQLSELECTBYID_HPP_
#define _DBO_ACTION_SQLSELECTBYID_HPP_

namespace dbo {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

struct SqlSelectByIdData {
	int params_ ;

	std::stringstream sql_ ;
	std::vector<std::string> join ;

	SqlSelectByIdData()
		:	params_(0)
	{}
} ;

template<class C>
class SqlSelectById
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	SqlSelectById(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;

	std::shared_ptr<SqlInsertData> data_ ;

	SqlSelectById(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlInsertData> data) ;

	template <class D> friend class SqlSelectById ;
};

}}

#endif
