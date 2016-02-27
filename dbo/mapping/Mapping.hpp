#ifndef _DBO_MAPPING_MAPPING_H_
#define _DBO_MAPPING_MAPPING_H_

#include <dbo/mapping/MappingInfo.h>

namespace dbo {
class connection ;

namespace mapping {

template<class C>
class Mapping : public MappingInfo
{
public:
	Mapping() ;
	virtual ~Mapping() ;

	virtual void init(connection& conn) ;
} ;

}}

#endif
