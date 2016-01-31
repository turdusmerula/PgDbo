// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_POSTGIS_SQL_TRAITS_H_
#define DBO_POSTGIS_SQL_TRAITS_H_

#include <string>
#include <vector>
#include <dbo/SqlTraits.h>
#include <dbo/SqlStatement.h>
#include <dbo/Exception.h>

#include <boost/optional.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_enum.hpp>

#include <gdal/ogrsf_frmts.h>

namespace dbo {

class  OGRException : public std::exception
{
public:
  /*! \brief Constructor.
   */
	OGRException(const std::string& msg, OGRErr code);

    virtual const char* what() const _GLIBCXX_USE_NOEXCEPT;

private:
	std::string msg_ ;
	OGRErr code_ ;

	std::string message_ ;
};

template<>
struct  sql_value_traits<OGRPoint, void>
{
  static const bool specialized = true;

  static std::string type(SqlConnection *conn, int size);
  static void bind(const OGRPoint& v, SqlStatement *statement, int column,
		   int size);
  static bool read(OGRPoint& v, SqlStatement *statement, int column,
		   int size);
};


}

#endif // DBO_POSTGIS_SQL_TRAITS_H_
