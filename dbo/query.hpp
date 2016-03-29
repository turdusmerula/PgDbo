#ifndef _DBO_QUERY_HPP_
#define _DBO_QUERY_HPP_

#include <dbo/stmt/PreparedStatement.h>

namespace dbo {
template <class T> class ptr ;

class query
{
public:

	query(connection& conn, const std::string& sql="") ;

//	query(const query& other) ;

	~query() ;

	query& operator=(const query& other) ;


//	template <class C>
//	query& select() ;
//
//	query& where(const std::string& condition) ;

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

	/**
	 * Execute the query
	 */
	query& execute() ;

	/**
	 * Reset bindings
	 */
	query& reset() ;

	template <class C>
	query& read(ptr<C>& ptr) ;

	template <class T>
	query& read(T& value) ;

	bool nextRow() ;

	size_t numRow() const ;

	const std::string& sql() const { return stmt_.sql() ; }
	void sql(const std::string& sql) ;

	bool hasrow() const { return hasrow_ ; }

	bool prepared() const { return stmt_.prepared() ; }

	stmt::PreparedStatement& stmt() { return stmt_ ; } ;
protected:
	connection* conn_ ;
	stmt::PreparedStatement stmt_ ;
	bool prepared_ ;

	bool hasrow_ ;
} ;

}

#endif
