/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <dbo/ogr/ogr_sql_traits.hpp>
#include <dbo/ogr/OGRException.h>

#include <boost/lexical_cast.hpp>
#include <string>

#ifndef DOXYGEN_ONLY

namespace dbo {
namespace traits {


void hex_to_bytes(std::vector<unsigned char>& hexstr, std::vector<unsigned char>& bytes)
{
    bytes.clear() ;
    for(std::string::size_type i = 0; i < hexstr.size() / 2; ++i)
    {
        std::string buff={(char)hexstr[i*2], (char)hexstr[i*2+1], '\0'} ;
        std::istringstream iss(buff) ;
        unsigned int n ;
        iss >> std::hex >> n ;
        bytes.push_back(static_cast<unsigned char>(n)) ;
    }
}

/*
 * OGRPoint
 */
std::string sql_value_traits<OGRPoint>::type(int size)
{
	//TODO: there seem to be a problem with GDAL when storing points with an SRID, the decoding of the Wkb string gives wrong results
	//return "geometry(POINT,4326) not null" ;
	return "geometry(POINT) not null" ;
}

void sql_value_traits<OGRPoint>::bind(const OGRPoint& v, stmt::Statement& statement, int size)
{
	// create wkb point
	char* wkbbuff=NULL ;
	v.exportToWkt(&wkbbuff) ;
	std::stringstream str ;
	//str << "SRID=4326;" << wkbbuff ;
	str << wkbbuff ;
	delete wkbbuff ;

	statement.bind(str.str()) ;
}

bool sql_value_traits<OGRPoint>::read(OGRPoint& v, stmt::Statement& statement, int size)
{
	std::vector<unsigned char> hex ;

	bool result=statement.read(hex) ;
	if(result)
	{
		std::vector<unsigned char> bytes ;

		hex_to_bytes(hex, bytes) ;

		OGRErr res=v.importFromWkb(bytes.data()) ;
		if(res!=OGRERR_NONE)
			throw ogr::OGRException("importFromWkb failed", res) ;
	}
	return result ;
}

}}

#endif
