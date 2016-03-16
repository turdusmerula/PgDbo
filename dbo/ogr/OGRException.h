#ifndef _DBO_OGR_OGREXCEPTION_H_
#define _DBO_OGR_OGREXCEPTION_H_

#include <string>

#include <gdal/ogrsf_frmts.h>

namespace dbo {
namespace ogr {

class OGRException : public std::exception
{
public:
	/*! \brief Constructor.
	 */
	OGRException(const std::string& msg, OGRErr code) ;

	virtual const char* what() const _GLIBCXX_USE_NOEXCEPT ;

private:
	std::string msg_ ;
	OGRErr code_ ;

	std::string message_ ;
};

}}

#endif
