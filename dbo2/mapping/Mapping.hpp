#ifndef _DBO_MAPPING_MAPPING_H_
#define _DBO_MAPPING_MAPPING_H_

#include <dbo2/mapping/MappingInfo.h>

namespace dbo2 {
class database ;

namespace mapping {

template<class C>
class Mapping : public MappingInfo
{
public:
	Mapping() ;
	virtual ~Mapping() ;

	virtual void init(database& conn) ;
} ;

}}

#endif
