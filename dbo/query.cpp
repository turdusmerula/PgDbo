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
	stmt_.sql(value) ;
}

size_t query::numRow() const
{
	return stmt_.numRow() ;
}

query::iterator query::begin()
{
	return query::iterator(this) ;
}

query::iterator query::end()
{
	return query::iterator() ;
}


// ----------------------------------------------------------------------------
query::row::row()
	:	query_(nullptr)
{
}

query::row::row(const query::row& row)
	:	query_(row.query_)
{
}

query::row::row(query* query)
	:	query_(query)
{
}


// ----------------------------------------------------------------------------
query::iterator::iterator()
	:	query_(nullptr)
{
}

query::iterator::iterator(query* query)
	:	query_(query),
		row_(query_)
{
	// move to first row
	query_->firstRow() ;
}

query::iterator::iterator(const iterator& other)
	:	query_(other.query_),
		row_(other.row_)
{
}

query::iterator::~iterator()
{
}

query::iterator& query::iterator::operator=(const query::iterator& other)
{
	query_ = other.query_ ;
	row_ = other.row_ ;
	return *this ;
}

query::row& query::iterator::operator*()
{
	return row_ ;
}

query::row* query::iterator::operator->()
{
	return &row_ ;
}

bool query::iterator::operator==(const query::iterator& other) const
{
	return query_==other.query_ || end()==other.end() ;
}

bool query::iterator::operator!=(const query::iterator& other) const
{
	return !operator==(other) ;
}

query::iterator& query::iterator::operator++()
{
	if(query_)
		query_->nextRow() ;
	return *this ;
}

query::iterator query::iterator::operator++(int inc)
{
	if(query_)
		for(int i=0 ; i<inc ; i++)
			query_->nextRow() ;
	return *this ;
}

bool query::iterator::end() const
{
	return query_==nullptr || query_->hasrow()==false ;
}
