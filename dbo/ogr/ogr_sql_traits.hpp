#ifndef DBO_OGR_SQL_TRAITS_H_
#define DBO_OGR_SQL_TRAITS_H_

#include <dbo/traits/sql_value_traits.hpp>
#include <dbo/stmt/Statement.h>

#include <gdal/ogrsf_frmts.h>

namespace dbo {
namespace traits {

template<>
struct sql_value_traits<OGRPoint, void>
{
	static const bool specialized=true ;

	static std::string type(int size) ;
	static void bind(const OGRPoint& v, stmt::Statement& statement, int size) ;
	static bool read(OGRPoint& v, stmt::Statement& statement, int size) ;
};

}}

#endif // DBO_POSTGIS_SQL_TRAITS_H_
