#include <dbo/ogr/OGRException.h>

#include <gdal/ogrsf_frmts.h>

using namespace dbo::ogr ;

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
