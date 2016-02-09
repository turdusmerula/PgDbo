#ifndef _DBO_MAPPING_SETINFO_H_
#define _DBO_MAPPING_SETINFO_H_

namespace dbo2 {
namespace mapping {

/*! \brief Type of an SQL relation.
 *
 * \ingroup dbo
 */
enum RelationType
{
	ManyToOne,  //!< Many-to-One relationship
	ManyToMany  //!< Many-to-Many relationship
};

struct SetInfo
{
	std::string tableName ;
	std::string joinName ;
	std::string joinSelfId, joinOtherId ;
	RelationType type ;
	int fkConstraints, otherFkConstraints ;

	SetInfo(std::string aTableName, RelationType type, const std::string& aJoinName, const std::string& aJoinSelfId, int someFkConstraints) ;
};

}}

#endif
