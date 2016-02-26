#ifndef _DBO_QUERY_HPP_
#define _DBO_QUERY_HPP_

#include <dbo2/stmt/PreparedStatement.h>

namespace dbo2 {
template <class T> class ptr ;

class query
{
public:

	query(connection& conn, const std::string& sql="") ;

//	query(const query& other) ;

	~query() ;

	query& operator=(const query& other) ;

//	query<Result>& where(const std::string& condition) ;
//
//	query<Result>& orderBy(const std::string& fieldName) ;
//	query<Result>& groupBy(const std::string& fields) ;
//	query<Result>& having(const std::string& fields) ;
//
//	query<Result>& offset(int count) ;
//	query<Result>& offset(const std::string& count) ;
//
//	query<Result>& limit(int count) ;
//	query<Result>& limit(const std::string& count) ;

	template <class C>
	query& bind(const C& value) ;

	/**
	 * Prepare the query.
	 * Once a query is prepared it becomes a named prepared statement and can not be modified
	 * Statements not prepared are executed as anonymous named statements.
	 */
	query& prepare() ;

	query& execute() ;

	template <class C>
	query& read(ptr<C>& ptr) ;

	template <class T>
	query& read(T& value) ;

	bool nextRow() ;

	const std::string& sql() { return sql_ ; }

	bool hasrow() const { return hasrow_ ; }
protected:
	connection* conn_ ;
	stmt::PreparedStatement stmt_ ;
	std::string sql_ ;
	bool prepared_ ;

	bool hasrow_ ;
} ;

}

#endif
