#ifndef _DBO_MAPPING_SETINFO_H_
#define _DBO_MAPPING_SETINFO_H_

#include <sstream>

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
	std::string joinSelfId ;
	std::string joinOtherId ;
	RelationType type ;
	int fkConstraints ;
	int otherFkConstraints ;

//	SetInfo(std::string aTableName, RelationType type, const std::string& aJoinName, const std::string& aJoinSelfId, int someFkConstraints) ;

	std::string debug(int tab)
	{
		std::stringstream ss ;
		std::string stab(tab, ' ') ;
		std::string stab1(tab+1, ' ') ;

		ss << "<SetInfo>" << std::endl ;
		ss << stab1 << "tableName: " << tableName << std::endl ;
		ss << stab1 << "joinName: " << joinName << std::endl ;
		ss << stab1 << "joinSelfId: " << joinSelfId << std::endl ;
		ss << stab1 << "joinOtherId: " << joinOtherId << std::endl ;

		switch(type)
		{
		case ManyToOne:
			ss << stab1 << "type: " << "ManyToOne" << std::endl ;
			break ;
		case ManyToMany:
			ss << stab1 << "type: " << "ManyToMany" << std::endl ;
			break ;
		}

		ss << stab1 << "fkConstraints: " << fkConstraints << std::endl ;
		ss << stab1 << "otherFkConstraints: " << otherFkConstraints << std::endl ;

		return ss.str() ;
	}
};

}}

#endif
