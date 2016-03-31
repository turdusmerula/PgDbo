#include <dbo/query.hpp>
#include <dbo/traits/std_sql_traits.hpp>
#include <dbo/Exception.h>

using namespace dbo ;

query::query(connection& conn, const std::string& sql)
	:	conn_(&conn),
		stmt_(conn, sql),
		prepared_(false),
		hasrow_(false)
{

}

//query::query(const query& other)
//	:	conn_(other.conn_),
//		sql_(other.sql_),
//		stmt_(other.conn_, other.sql_),
//		prepared_(false)
//{
//}

query::~query()
{

}

query& query::operator=(const query& other)
{
	conn_ = other.conn_ ;
	stmt_ = other.stmt_ ;
	prepared_ = other.prepared_ ;
	hasrow_ = other.hasrow_ ;

	return *this ;
}

query& query::prepare()
{
	stmt_.hashName(true) ;
	stmt_.prepare() ;
	prepared_ = true ;

	return *this ;
}

query& query::execute()
{
	if(prepared_==false)
	{
		// prepare as an anonymous statement
		stmt_.name("") ;
		stmt_.prepare() ;
	}

	stmt_.execute() ;

	hasrow_ = stmt_.nextRow() ;

	return *this ;
}

query& query::reset()
{
	stmt_.reset() ;

	return *this ;
}

bool query::firstRow()
{
	hasrow_ = stmt_.firstRow() ;
	return hasrow_ ;
}

bool query::nextRow()
{
	hasrow_ = stmt_.nextRow() ;
	return hasrow_ ;
}

void query::sql(const std::string& value)
{
//	if(prepared_==true)
//		throw Exception("Query statement already preapred") ;
//
	stmt_.sql(value) ;
}

size_t query::numRow() const
{
	return stmt_.numRow() ;
}
