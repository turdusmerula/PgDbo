/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <dbo2/traits/StdSqlTraits.h>
#include <dbo2/traits/SqlPostgresTypes.hpp>

#include <boost/lexical_cast.hpp>
#include <string>

#ifndef DOXYGEN_ONLY

namespace dbo2 {
namespace traits {

/*
 * std::string
 */
std::string sql_value_traits<std::string>::type(int size)
{
	return SqlPostgresTypes::textType(size)+" not null" ;
}

//void sql_value_traits<std::string>::bind(const std::string& v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<std::string>::read(std::string& v, SqlStatement *statement, int column, int size)
//{
//	if(!statement->getResult(column, &v, size))
//	{
//		v.clear();
//		return false;
//	}
//	else
//		return true;
//}

/*
 * long long
 */

std::string sql_value_traits<long long>::type(int size)
{
	return SqlPostgresTypes::longLongType()+" not null" ;
}

//void sql_value_traits<long long>::bind(long long v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<long long>::read(long long& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v);
//}

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

//void sql_value_traits<long>::bind(long v, SqlStatement *statement, int column, int size)
//{
//	if(sizeof(long)==4)
//		statement->bind(column, (int)v);
//	else
//		statement->bind(column, (long long)v);
//}
//
//bool sql_value_traits<long>::read(long& v, SqlStatement *statement, int column, int size)
//{
//	if(sizeof(long)==4)
//	{
//		int v2;
//		bool result = statement->getResult(column, &v2);
//		v = v2;
//		return result;
//	}
//	else
//	{
//		long long v2;
//		bool result = statement->getResult(column, &v2);
//		v = v2;
//		return result;
//	}
//}

/*
 * int
 */

std::string sql_value_traits<int>::type(int size)
{
	return "integer not null" ;
}

//void sql_value_traits<int>::bind(int v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<int>::read(int& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v);
//}

/*
 * short
 */

std::string sql_value_traits<short>::type(int size)
{
	return "smallint not null" ;
}

//void sql_value_traits<short>::bind(short v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<short>::read(short& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v);
//}

/*
 * bool
 */

std::string sql_value_traits<bool>::type(int size)
{
	return SqlPostgresTypes::booleanType()+" not null" ;
}

//void sql_value_traits<bool>::bind(bool v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v?1:0);
//}
//
//bool sql_value_traits<bool>::read(bool& v, SqlStatement *statement, int column, int size)
//{
//	int intValue;
//	bool result = statement->getResult(column, &intValue);
//	if(result)
//		v = intValue?true:false;
//
//	return result;
//}

/*
 * float
 */

std::string sql_value_traits<float>::type(int size)
{
	return "real not null" ;
}

//void sql_value_traits<float>::bind(float v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<float>::read(float& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v);
//}

/*
 * double
 */

std::string sql_value_traits<double>::type(int size)
{
	return "double precision not null" ;
}

//void sql_value_traits<double>::bind(double v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<double>::read(double& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v);
//}

/*
 * size_t
 */

std::string sql_value_traits<size_t>::type(int size)
{
	return "double precision not null" ;
}

//void sql_value_traits<size_t>::bind(size_t v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, static_cast<long long>(v));
//}
//
//bool sql_value_traits<size_t>::read(size_t& v, SqlStatement *statement, int column, int size)
//{
//	long long val ;
//	bool res=statement->getResult(column, &val) ;
//	v = static_cast<size_t>(val) ;
//	return res ;
//}

/*
 * boost::posix_time::ptime
 */

std::string sql_value_traits<boost::posix_time::ptime>::type(int size)
{
	return SqlPostgresTypes::dateTimeType(SqlPostgresTypes::SqlDateTime) ;
}

//void sql_value_traits<boost::posix_time::ptime>::bind(const boost::posix_time::ptime& v, SqlStatement *statement, int column, int size)
//{
//	if(v.is_special())
//		statement->bindNull(column);
//	else
//		statement->bind(column, v, SqlDateTime);
//}
//
//bool sql_value_traits<boost::posix_time::ptime>::read(boost::posix_time::ptime& v, SqlStatement *statement, int column, int size)
//{
//	if(statement->getResult(column, &v, SqlDateTime))
//		return true;
//	else
//	{
//		v = boost::posix_time::ptime();
//		return false;
//	}
//}

/*
 * boost::posix_time::time_duration
 */

std::string sql_value_traits<boost::posix_time::time_duration>::type(int size)
{
	return SqlPostgresTypes::dateTimeType(SqlPostgresTypes::SqlTime) ;
}

//void sql_value_traits<boost::posix_time::time_duration>::bind(const boost::posix_time::time_duration& v, SqlStatement *statement, int column, int size)
//{
//	if(v.is_special())
//		statement->bindNull(column);
//	else
//		statement->bind(column, v);
//}
//
//bool sql_value_traits<boost::posix_time::time_duration>::read(boost::posix_time::time_duration& v, SqlStatement *statement, int column, int size)
//{
//	if(statement->getResult(column, &v))
//		return true;
//	else
//	{
//		v = boost::posix_time::time_duration(boost::posix_time::not_a_date_time);
//		return false;
//	}

/*
 * std::vector<unsigned char>
 */

std::string sql_value_traits< std::vector<unsigned char> >::type(int size)
{
	return SqlPostgresTypes::blobType() ;
}

//void sql_value_traits<std::vector<unsigned char> >::bind(const std::vector<unsigned char>& v, SqlStatement *statement, int column, int size)
//{
//	statement->bind(column, v);
//}
//
//bool sql_value_traits<std::vector<unsigned char> >::read(std::vector<unsigned char>& v, SqlStatement *statement, int column, int size)
//{
//	return statement->getResult(column, &v, size);
//}

}}


#endif
