#ifndef _DBO_STMT_BULKSTATEMENT_HPP_
#define _DBO_STMT_BULKSTATEMENT_HPP_

#include <string>
#include <deque>
#include <map>
#include <memory>
#include <sstream>

#include <dbo/stmt/Statement.h>

extern "C"
{
struct pg_result ;
}

namespace dbo {
class connection ;

namespace stmt {

class BulkStatement : public Statement
{
public:
	/**
	 * Build a statement, the name of the statement will be created from the hash of the sql query
	 */
	BulkStatement(connection& conn, const std::string& table) ;

	virtual ~BulkStatement() ;

	/**
	 * Bind a null value
	 */
	virtual void bind() ;

	/**
	 * Bind a string value
	 */
	virtual void bind(const std::string& value) ;

	/**
	 * Bind a binary value
	 */
	virtual void bind(const std::vector<unsigned char>& value) ;

	/**
	 * Read content to string
	 */
	virtual bool read(char*& value) ;

	/**
	 * Read content to binary array
	 */
	virtual bool read(std::vector<unsigned char>& value) ;

	/**
	 * Execute the prepared statement
	 */
	virtual void execute() ;

	void addColumn(const std::string& name) ;

	void prepare() ;

	bool nextRow() ;

protected:
	size_t row_ ;
	size_t column_ ;
	std::string sep_ ;
	std::stringstream sql_ ;

	static const size_t BuffSize=sizeof(int)/2-1 ;
	struct Buffer
	{
		unsigned char data_[BuffSize] ;
		size_t size_ ;

		Buffer()
			: size_(0)
		{}
	} ;
	std::deque<Buffer> buffers_ ;

	std::string table_ ;
	std::vector<std::string> columns_ ;

	std::string escape(const std::string& value) ;
	std::string escapeHex(const std::vector<unsigned char>& value) ;

	static void pg_result_deleter(pg_result* result) ;

	void addData(const char* data) ;
	void addData(const std::string& data) ;
} ;

}}

#endif
