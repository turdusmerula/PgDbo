/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */


#include <dbo/traits/sql_value_traits.hpp>
#include <dbo/traits/std_sql_traits.hpp>
#include <dbo/traits/sql_postgres_types.hpp>

#include <dbo/stmt/Statement.h>

#include <boost/lexical_cast.hpp>
#include <string>

#ifndef DOXYGEN_ONLY

namespace dbo {
namespace traits {

/*
 * std::string
 */
std::string sql_value_traits<std::string>::type(int size)
{
	return SqlPostgresTypes::textType(size)+" not null" ;
}

void sql_value_traits<std::string>::bind(const std::string& v, stmt::Statement& statement, int size)
{
	statement.bind(v) ;
}

bool sql_value_traits<std::string>::read(std::string& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = res ;
		return true ;
	}
	return false ;
}

/*
 * long long
 */
std::string sql_value_traits<long long>::type(int size)
{
	return SqlPostgresTypes::longLongType()+" not null" ;
}

void sql_value_traits<long long>::bind(long long v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<long long>::read(long long& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<long long>(res) ;
		return true ;
	}
	return false ;
}

/*
 * int
 */
std::string sql_value_traits<int>::type(int size)
{
	return "integer not null" ;
}

void sql_value_traits<int>::bind(int v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<int>::read(int& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<int>(res) ;
		return true ;
	}
	return false ;
}

/*
 * long
 */

std::string sql_value_traits<long>::type(int size)
{
	if(sizeof(long)==4)
		return "integer not null" ;
	else
		return SqlPostgresTypes::longLongType()+" not null" ;
}

void sql_value_traits<long>::bind(long v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<long>::read(long& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<long>(res) ;
		return true ;
	}
	return false ;
}



/*
 * short
 */

std::string sql_value_traits<short>::type(int size)
{
	return "smallint not null" ;
}

void sql_value_traits<short>::bind(short v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<short>::read(short& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<long>(res) ;
		return true ;
	}
	return false ;
}

/*
 * bool
 */

std::string sql_value_traits<bool>::type(int size)
{
	return SqlPostgresTypes::booleanType()+" not null" ;
}

void sql_value_traits<bool>::bind(bool v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<bool>::read(bool& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = (res[0]=='t') ;
		return true ;
	}
	return false ;
}

/*
 * float
 */

std::string sql_value_traits<float>::type(int size)
{
	return "real not null" ;
}

void sql_value_traits<float>::bind(float v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<float>::read(float& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<float>(res) ;
		return true ;
	}
	return false ;
}

/*
 * double
 */

std::string sql_value_traits<double>::type(int size)
{
	return "double precision not null" ;
}

void sql_value_traits<double>::bind(double v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<double>::read(double& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<double>(res) ;
		return true ;
	}
	return false ;
}

/*
 * size_t
 */

std::string sql_value_traits<size_t>::type(int size)
{
	return "double precision not null" ;
}

void sql_value_traits<size_t>::bind(size_t v, stmt::Statement& statement, int size)
{
	statement.bind(boost::lexical_cast<std::string>(v)) ;
}

bool sql_value_traits<size_t>::read(size_t& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::lexical_cast<size_t>(res) ;
		return true ;
	}
	return false ;
}


/*
 * boost::gregorian::date
 */
std::string sql_value_traits<boost::gregorian::date>::type(int size)
{
	return SqlPostgresTypes::dateTimeType(SqlPostgresTypes::SqlDate) ;
}

void sql_value_traits<boost::gregorian::date>::bind(const boost::gregorian::date& v, stmt::Statement& statement, int size)
{
	if(v.is_special())
		statement.bind() ;
	else
		statement.bind(boost::gregorian::to_iso_extended_string(v)) ;
}

bool sql_value_traits<boost::gregorian::date>::read(boost::gregorian::date& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::gregorian::from_string(res) ;
		return true ;
	}
	v = boost::gregorian::date(boost::gregorian::not_a_date_time) ;
	return false ;
}


/*
 * boost::posix_time::ptime
 */
std::string sql_value_traits<boost::posix_time::ptime>::type(int size)
{
	return SqlPostgresTypes::dateTimeType(SqlPostgresTypes::SqlDateTime) ;
}

void sql_value_traits<boost::posix_time::ptime>::bind(const boost::posix_time::ptime& v, stmt::Statement& statement, int size)
{
	if(v.is_special())
		statement.bind() ;
	else
	{
		/*
		 * Add explicit timezone offset. Postgres will ignore this for a TIMESTAMP
		 * column, but will treat the timestamp as UTC in a TIMESTAMP WITH TIME
		 * ZONE column -- possibly in a legacy table.
		 */
		statement.bind(boost::posix_time::to_iso_extended_string(v)+"+00") ;
	}
}

bool sql_value_traits<boost::posix_time::ptime>::read(boost::posix_time::ptime& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		std::string s=res ;
		/*
		 * Handle timezone offset. Postgres will append a timezone offset [+-]dd
		 * if a column is defined as TIMESTAMP WITH TIME ZONE -- possibly
		 * in a legacy table. If offset is present, subtract it for UTC output.
		 */
		if(s.size()>=3 && std::strchr("+-", s[s.size()-3]))
		{
			int hours=boost::lexical_cast<int>(s.substr(s.size()-3)) ;
			boost::posix_time::time_duration offset=boost::posix_time::hours(hours) ;
			v = boost::posix_time::time_from_string(s.substr(0, s.size()-3))-offset ;
		}
		else
			v = boost::posix_time::time_from_string(s) ;
		return true ;
	}
	v = boost::posix_time::ptime(boost::posix_time::not_a_date_time) ;
	return false ;
}

/*
 * boost::posix_time::time_duration
 */

std::string sql_value_traits<boost::posix_time::time_duration>::type(int size)
{
	return SqlPostgresTypes::dateTimeType(SqlPostgresTypes::SqlTime) ;
}

void sql_value_traits<boost::posix_time::time_duration>::bind(const boost::posix_time::time_duration& v, stmt::Statement& statement, int size)
{
	if(v.is_special())
		statement.bind() ;
	else
		statement.bind(boost::posix_time::to_simple_string(v)) ;
}

bool sql_value_traits<boost::posix_time::time_duration>::read(boost::posix_time::time_duration& v, stmt::Statement& statement, int size)
{
	char* res ;
	if(statement.read(res))
	{
		v = boost::posix_time::time_duration(boost::posix_time::duration_from_string(res)) ;
		return true ;
	}

	v = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
	return false;
}

/*
 * std::vector<unsigned char>
 */

std::string sql_value_traits< std::vector<unsigned char> >::type(int size)
{
	return SqlPostgresTypes::blobType() ;
}

void sql_value_traits<std::vector<unsigned char> >::bind(const std::vector<unsigned char>& v, stmt::Statement& statement, int size)
{
	statement.bind(v) ;
}

bool sql_value_traits<std::vector<unsigned char>>::read(std::vector<unsigned char>& v, stmt::Statement& statement, int size)
{
	return statement.read(v) ;
}

}}


#endif
