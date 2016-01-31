/*
 * Copyright (C) 2009 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <dbo/PostgisSqlTraits.h>
#include <dbo/SqlStatement.h>

#include <boost/lexical_cast.hpp>
#include <string>

#ifndef DOXYGEN_ONLY

namespace dbo {

OGRException::OGRException(const std::string& _msg, OGRErr _code)
	: std::exception()
{
	msg_ = _msg ;
	code_ = _code ;

	std::string strcode ;
	switch(code_)
	{
	case OGRERR_NONE:
		strcode = "OGRERR_NONE" ;
		break ;
	case OGRERR_NOT_ENOUGH_DATA:
		strcode = "OGRERR_NOT_ENOUGH_DATA" ;
		break ;
	case OGRERR_NOT_ENOUGH_MEMORY:
		strcode = "OGRERR_NOT_ENOUGH_MEMORY" ;
		break ;
	case OGRERR_UNSUPPORTED_GEOMETRY_TYPE:
		strcode = "OGRERR_UNSUPPORTED_GEOMETRY_TYPE" ;
		break ;
	case OGRERR_UNSUPPORTED_OPERATION:
		strcode = "OGRERR_UNSUPPORTED_OPERATION" ;
		break ;
	case OGRERR_CORRUPT_DATA:
		strcode = "OGRERR_CORRUPT_DATA" ;
		break ;
	case OGRERR_FAILURE:
		strcode = "OGRERR_FAILURE" ;
		break ;
	case OGRERR_UNSUPPORTED_SRS:
		strcode = "OGRERR_UNSUPPORTED_SRS" ;
		break ;
	case OGRERR_INVALID_HANDLE:
		strcode = "OGRERR_INVALID_HANDLE" ;
		break ;
	default:
		strcode = "??" ;
	}
	message_ = "OGR failed with code "+strcode+": "+msg_ ;
}

const char* OGRException::what() const _GLIBCXX_USE_NOEXCEPT
{
	return message_.c_str() ;
}

void hex_to_bytes(std::vector<unsigned char>& hexstr, std::vector<unsigned char>& bytes)
{
    bytes.clear();
    for(std::string::size_type i = 0; i < hexstr.size() / 2; ++i)
    {
        std::string buff={(char)hexstr[i*2], (char)hexstr[i*2+1], '\0'} ;
        std::istringstream iss(buff) ;
        unsigned int n;
        iss >> std::hex >> n;
        bytes.push_back(static_cast<unsigned char>(n));
    }
}

/*
 * OGRPoint
 */
std::string sql_value_traits<OGRPoint>::type(SqlConnection *conn, int size)
{
	//TODO: there seem to be a problem with GDAL when storing points with an SRID, the decoding of the Wkb string gives wrong results
	//return "geometry(POINT,4326) not null" ;
	return "geometry(POINT) not null" ;
}

void sql_value_traits<OGRPoint>::bind(const OGRPoint& v, SqlStatement *statement, int column, int size)
{
	// create wkb point
	char* wkbbuff=NULL ;
	v.exportToWkt(&wkbbuff) ;
	std::stringstream str ;
	//str << "SRID=4326;" << wkbbuff ;
	str << wkbbuff ;
	delete wkbbuff ;

	statement->bind(column, str.str());
}

bool sql_value_traits<OGRPoint>::read(OGRPoint& v, SqlStatement *statement, int column, int size)
{
	std::vector<unsigned char> hex ;

	bool result=statement->getResult(column, &hex, 0);
	if(result)
	{
		std::vector<unsigned char> bytes ;

		hex_to_bytes(hex, bytes) ;

		OGRErr res=v.importFromWkb(bytes.data()) ;
		if(res!=OGRERR_NONE)
			throw OGRException("importFromWkb failed", res) ;
	}
	return result ;
}

}

#endif
