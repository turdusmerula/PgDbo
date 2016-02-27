#ifndef _DBO_STMT_PREPAREDSTATEMENT_H_
#define _DBO_STMT_PREPAREDSTATEMENT_H_

#include <string>
#include <vector>
#include <map>
#include <memory>

#include <dbo2/stmt/Statement.h>

extern "C"
{
struct pg_result ;
}

namespace dbo2 {
class connection ;

namespace stmt {

class PreparedStatement : public Statement
{
public:
	/**
	 * Build a statement, the name of the statement will be created from the hash of the sql query
	 */
	PreparedStatement(connection& conn, std::string sql) ;

	/**
	 * Build a statement. If the given name is empty then the statement is treated as an anonymous
	 * statement and will overwrite any previous anonymous statement
	 */
	PreparedStatement(connection& conn, std::string name, std::string sql) ;

	virtual ~PreparedStatement() ;

//	Statement(const Statement& other) ;

	/**
	 * A statement is copyable under conditions.
	 * You can only copy prepared statements, statements with results will throw an error.
	 * You can copy a statement only if it is from the same connection
	 */
//	Statement& operator=(const Statement& other) ;

	/**
	 * Bind a null value
	 */
	void bind() ;

	/**
	 * Bind a string value
	 */
	void bind(const std::string& value) ;

	/**
	 * Bind a binary value
	 */
	void bind(const std::vector<unsigned char>& value) ;

	/**
	 * Read content to string
	 */
	bool read(char*& value) ;

	/**
	 * Read content to binary array
	 */
	bool read(std::vector<unsigned char>& value) ;

	/**
	 * Prepare the statement and registers it
	 * Uses the bound values to chose the right oids
	 * NULL values binds to OIDDefault
	 * bind count must be equal to request parameters number
	 */
	void prepare() ;

	/**
	 * Clear the statement bound values and current result
	 */
	void reset() ;

	/**
	 * Execute the prepared statement
	 */
	void execute() ;

	bool nextRow() ;

	/**
	 * Indicate if an insert has a returning
	 */
	bool hasReturning() { return hasReturning_ ; }

	const std::string sql() const { return rawsql_ ; }
	void sql(const std::string& sql) ;

	const std::string name() const { return name_ ; }

	/**
	 * Set name of prepared statement
	 * If name is empty then prepared statement is anonymous and will overwrite previous
	 * prepared anonymous statement at prepare.
	 */
	void name(const std::string& name) ;

	bool hashName() { return hashname_ ; }
	void hashName(bool value) ;

	bool prepared() const { return prepared_ ; }
	connection& conn() const { return *conn_ ; }
protected:
	enum OIDEnum
	{
		OIDDefault = 0,
		OIDBytea = 17
	} ;

	bool hashname_ ;
	std::string name_ ;

	std::string rawsql_ ;
	std::string sql_ ;

	bool prepared_ ;
	size_t paramCount_ ;

	// allow statement to be copyable and change the pg_result to RAII object with pg_result_deleter as custom deleter
	std::shared_ptr<pg_result> result_ ;

	int row_ ;
	int column_ ;
	size_t affectedRows_ ;
	bool hasReturning_ ;

	std::vector<unsigned int> oids_ ;

	std::vector<std::string> svalues_ ;	// RAII version of values
	std::vector<const char*> values_ ;
	std::vector<int> lengths_ ;
	std::vector<int> formats_ ;

	std::string convertToNumberedPlaceholders(const std::string& sql) ;
	size_t getNumberPlaceHolders(const std::string& sql) ;
	size_t getPlaceHolders(const std::string& sql, std::map<std::string, std::string>& placeholders) ;
	std::string getBoundPlaceholders() ;

	static void pg_result_deleter(pg_result* result) ;
} ;

}}

#endif
