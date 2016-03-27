#ifndef _DBO_ACTION_SQLSELECTBYID_HPP_
#define _DBO_ACTION_SQLSELECTBYID_HPP_

#include <string>
#include <stack>
#include <sstream>
#include <vector>
#include <boost/lexical_cast.hpp>

#include <dbo/mapping/IdPrefixStack.h>

namespace dbo {

namespace action {

enum SqlSelectState {
	SelectColumns,
	IdJoinCondition,
	IdJoinNames,
	IdCondition
} ;

struct SqlSelectData {
	bool whereIdClause_ ;
	int params_ ;
	int ids_ ;

	int as_num_ ;
	std::stack<std::string> as_letter_ ;

	std::stringstream sql_ ;

	std::vector<std::string> id_names_ ;	// elements of the t1 id
	struct Join {
		std::string tableName_ ;
		std::string as_ ;
		std::vector<std::string> id_names_ ;	// elements of the tx id to match with t1
	} ;
	std::vector<Join> joins_ ;

	mapping::IdPrefixStack id_prefix_satck_ ;	// composite ids prefix is built from the parents names

	SqlSelectData()
		:	params_(0),
			as_num_(1),
			ids_(0)
	{}

	std::string as_letter() { return as_letter_.top() ; }
	void as_letter_push(const std::string& name) { as_letter_.push("\""+name+"\"") ; }
	void as_letter_push() { as_letter_.push("t"+boost::lexical_cast<std::string>(as_num_++)) ; }
	void as_letter_pop() { as_letter_.pop() ; } ;

} ;

template<class C>
class SqlSelect
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	SqlSelect(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, bool whereIdClause) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class E> void actCollection(const mapping::CollectionRef<E>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;

	std::shared_ptr<SqlSelectData> data_ ;

	SqlSelectState state_ ;

	SqlSelect(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlSelectData> data) ;

	template <class D> friend class SqlSelect ;
};

}}

#endif
