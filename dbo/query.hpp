#ifndef _DBO_QUERY_HPP_
#define _DBO_QUERY_HPP_

#include <dbo/stmt/PreparedStatement.h>

namespace dbo {
template <class T> class ptr ;

class query
{
public:
	class iterator ;
	class row ;

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

	iterator begin()  ;
	iterator end() ;

	/**
	 * Move to first available row
	 * @return false if no available row
	 */
	bool firstRow() ;

	/**
	 * Move to the next available row
	 * @return false if no available row
	 */
	bool nextRow() ;

	/**
	 * Return number of fetched row in case of a select
	 */
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

class query::row
{
public:
	row(const row& row) ;

	template <class C>
	row& read(ptr<C>& ptr) ;

	template <class T>
	row& read(T& value) ;

	friend query::iterator ;
protected:
	query* query_ ;

	row(query* query) ;
	row() ;
} ;

class query::iterator : public std::iterator<std::input_iterator_tag, query::row>
{
public:
	/*! \brief Copy constructor.
	 */
	iterator(const iterator& other) ;

	/*! \brief Destructor.
	 */
	~iterator() ;

	/*! \brief Assignment operator.
	 */
	iterator& operator=(const iterator& other) ;

	/*! \brief Dereference operator.
	 */
	row& operator*() ;

	/*! \brief Dereference operator.
	 */
	row* operator->() ;

	/*! \brief Comparison operator.
	 *
	 * Returns true if two iterators point to the same value in the
	 * same %collection, or point both to the end of a collection.
	 */
	bool operator==(const iterator& other) const ;

	/*! \brief Comparison operator.
	 */
	bool operator!=(const iterator& other) const ;

	/*! \brief Pre increment operator.
	 */
	iterator& operator++() ;

	/*! \brief Post increment operator.
	 */
	iterator operator++(int) ;

	friend query ;
private:
	query* query_ ;
	query::row row_ ;

	bool end() const ;

	iterator() ;
	iterator(query* query) ;

	friend class row ;
};

}

#endif
