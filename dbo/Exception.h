#ifndef _DBO_EXCEPTION_H_
#define _DBO_EXCEPTION_H_

#include <string>
#include <stdexcept>

namespace dbo {

class Exception : public std::runtime_error
{
public:
	Exception(const std::string& error, const std::string& code=std::string())
		: std::runtime_error(error),
		  code_(code)
	{}

	virtual ~Exception() throw()
	{}

	std::string code() const
	{
		return code_;
	}

private:
	std::string code_ ;
} ;

}

#endif
