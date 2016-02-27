#ifndef _DBO_MAPPING_FIELD_H_
#define _DBO_MAPPING_FIELD_H_

namespace dbo {
namespace mapping {

const int FKNotNull = 0x01;
const int FKOnUpdateCascade = 0x02;
const int FKOnUpdateSetNull = 0x04;
const int FKOnDeleteCascade = 0x08;
const int FKOnDeleteSetNull = 0x10;

}}

#endif
