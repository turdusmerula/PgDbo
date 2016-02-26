#ifndef _DBO_STMT_STATEMENT_HPP_
#define _DBO_STMT_STATEMENT_HPP_

#include <string>
#include <vector>
#include <map>
#include <memory>

namespace dbo2 {
class connection ;

namespace stmt {

class Statement
{
public:
	/**
	 * Build a statement, the name of the statement will be created from the hash of the sql query
	 */
	Statement(connection& conn) ;

	virtual ~Statement() ;

	/**
	 * Bind a null value
	 */
	virtual void bind()=0 ;

	/**
	 * Bind a string value
	 */
	virtual void bind(const std::string& value)=0 ;

	/**
	 * Bind a binary value
	 */
	virtual void bind(const std::vector<unsigned char>& value)=0 ;

	/**
	 * Read content to string
	 */
	virtual bool read(char*& value)=0 ;

	/**
	 * Read content to binary array
	 */
	virtual bool read(std::vector<unsigned char>& value)=0 ;

	/**
	 * Execute the prepared statement
	 */
	virtual void execute()=0 ;

protected:
	connection* conn_ ;

} ;

}}

#endif
