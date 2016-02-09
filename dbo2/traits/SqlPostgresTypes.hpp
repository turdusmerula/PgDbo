#ifndef _DBO_TRAITS_SQLDATABASETRAIT_H_
#define _DBO_TRAITS_SQLDATABASETRAIT_H_

#include <sstream>
#include <boost/lexical_cast.hpp>
#include <dbo2/Exception.h>

namespace dbo2 {
namespace traits {

class SqlPostgresTypes
{
public:

	/*! \brief Enum that defines a date time type.
	 */
	enum SqlDateTimeType
	{
		SqlDate,    	//!< Date only
		SqlDateTime,    //!< Date and time
		SqlTime     	//!< Time duration
	} ;

	/*! \brief Returns the 'autoincrement' SQL type.
	 *
	 * This is used by Session::createTables() to create the <i>id</i>
	 * column.
	 */
	static std::string autoincrementType()
	{
		return "bigserial" ;
	}

	/*! \brief Returns the suffix for an 'autoincrement' insert statement.
	 *
	 * This is appended to the <tt>insert</tt> statement, since some back-ends
	 * need to be indicated that they should return the autoincrement id.
	 */
	static std::string autoincrementInsertSuffix(const std::string& id)
	{
		return " returning \""+id+"\"" ;
	}

	/*! \brief Returns the date/time type.
	 *
	 * \sa SqlStatement::bind(int, const boost::posix_time::ptime&, SqlDateTimeType)
	 */
	static std::string dateTimeType(SqlDateTimeType type)
	{
		switch(type)
		{
		case SqlDate:
			return "date" ;
			break ;
		case SqlDateTime:
			return "timestamp" ;
			break ;
		case SqlTime:
			return "interval" ;
			break ;
		}

		std::stringstream ss ;
		ss << __FILE__ << ":" << __LINE__ << ": implementation error" ;
		throw Exception(ss.str()) ;
	}

	/*! \brief Returns the blob type.
	 *
	 * \sa SqlStatement::bind(int, const std::vector<unsigned char>&)
	 */
	static std::string blobType()
	{
		return "bytea not null" ;
	}

	/*! \brief Returns the text type.
	 *
	 * This is the text type for a string. If \p size = -1, then a type
	 * should be returned which does not require size information, otherwise
	 * a type should be returned that limits the size of the stored string
	 * to \p size.
	 *
	 * This method will return "text" by default if size = -1, and
	 * "varchar(size)" otherwise.
	 *
	 * \sa SqlStatement::bind(int column, const std::string& value)
	 */
	static std::string textType(int size)
	{
		if(size==-1)
			return "text" ;
		else
			return "varchar("+boost::lexical_cast<std::string>(size)+")" ;
	}

	/*! \brief Returns the 64-bit integer type.
	 *
	 * This method will return "bigint" by default.
	 *
	 */
	static std::string longLongType()
	{
		return "bigint" ;
	}

	/*! \brief Returns the boolean type.
	 *
	 * This method will return "boolean" by default.
	 */
	static std::string booleanType()
	{
		return "boolean" ;
	}

} ;

}}

#endif
