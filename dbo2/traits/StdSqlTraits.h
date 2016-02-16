/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef _DBO_TRAITS_STDSQLTRAITS_H_
#define _DBO_TRAITS_STDSQLTRAITS_H_

#include <string>
#include <vector>

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_enum.hpp>

#include <dbo2/traits/sql_value_traits.hpp>

#include <dbo2/stmt/Statement.h>

namespace dbo2 {
namespace stmt {
class Statement ;
}
namespace traits {

template<>
struct sql_value_traits<std::string, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const std::string& v, stmt::Statement& statement, int size) ;
	static bool read(std::string& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<long long, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(long long v, stmt::Statement& statement, int size) ;
	static bool read(long long& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<int, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(int v, stmt::Statement& statement, int size) ;
	static bool read(int& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<long, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(long v, stmt::Statement& statement, int size) ;
	static bool read(long& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<short, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(short v, stmt::Statement& statement, int size) ;
	static bool read(short& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<bool, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(bool v, stmt::Statement& statement, int size) ;
	static bool read(bool& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<float, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(float v, stmt::Statement& statement, int size) ;
	static bool read(float& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<double, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(double v, stmt::Statement& statement, int size) ;
	static bool read(double& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<size_t, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(size_t v, stmt::Statement& statement, int size) ;
	static bool read(size_t& v, stmt::Statement& statement, int size) ;
};

template<>
struct sql_value_traits<boost::gregorian::date, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const boost::gregorian::date& v, stmt::Statement& statement, int size) ;
	static bool read(boost::gregorian::date& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<boost::posix_time::ptime, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const boost::posix_time::ptime& v, stmt::Statement& statement, int size) ;
	static bool read(boost::posix_time::ptime& v, stmt::Statement& statement, int size) ;
} ;

template<>
struct sql_value_traits<boost::posix_time::time_duration, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const boost::posix_time::time_duration& v, stmt::Statement& statement, int size) ;
	static bool read(boost::posix_time::time_duration& v, stmt::Statement& statement, int size) ;
} ;

template<typename Enum>
struct sql_value_traits<Enum, typename boost::enable_if<boost::is_enum<Enum>>::type> : public sql_value_traits<int>
{
	static void bind(Enum v, stmt::Statement& statement, int size)
	{
		sql_value_traits<int>::bind(static_cast<int>(v), statement, size);
	}

	static bool read(Enum& v, stmt::Statement& statement, int size)
	{
		return sql_value_traits<int>::read(reinterpret_cast<int&>(v), statement, size) ;
	}
} ;

template<>
struct sql_value_traits<std::vector<unsigned char>, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const std::vector<unsigned char>& v, stmt::Statement& statement, int size);
	static bool read(std::vector<unsigned char>& v, stmt::Statement& statement, int size) ;
};

template<typename T>
struct sql_value_traits<boost::optional<T>, void>
{
	static const bool specialized=true ;

	static std::string type(int size)
	{
		std::string nested = sql_value_traits<T>::type(size) ;
		if(nested.length()>9 && nested.substr(nested.length()-9)==" not null")
			return nested.substr(0, nested.length()-9) ;
		else
			return nested ;
	}

	static void bind(const boost::optional<T>& v, stmt::Statement& statement, int size)
	{
		if(v)
			sql_value_traits<T>::bind(v.get(), statement, size) ;
		else
			statement.bind() ;
	}

	static bool read(boost::optional<T>& v, stmt::Statement& statement, int size)
	{
		T result ;
		if(sql_value_traits<T>::read(result, statement, size))
		{
			v = result ;
			return true ;
		}
		else
		{
			v = boost::optional<T>() ;
			return false ;
		}
	}
};

}}

#endif
