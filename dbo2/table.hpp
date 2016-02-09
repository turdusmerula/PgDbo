#ifndef _DBO_TABLE_H_
#define _DBO_TABLE_H_

#include <string>


namespace dbo2 {

class table
{
public:
	table() ;
	virtual ~table() ;

	virtual const std::string& name()=0 ;


} ;

}

#include <dbo2/table.cxx>

#endif
