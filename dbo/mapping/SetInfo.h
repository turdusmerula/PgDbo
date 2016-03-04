#ifndef _DBO_MAPPING_SETINFO_H_
#define _DBO_MAPPING_SETINFO_H_

#include <sstream>

namespace dbo {
/*! \brief Type of an SQL relation.
 *
 * \ingroup dbo
 */
enum RelationType
{
	OneToOne,  //!< One-to-One relationship
	ManyToOne,  //!< Many-to-One relationship
	ManyToMany  //!< Many-to-Many relationship
};

namespace mapping {

struct SetInfo
{
	std::string tableName_ ;
	std::string joinName_ ;
	std::string joinSelfId_ ;
	std::string joinOtherId_ ;
	RelationType type_ ;
	int fkConstraints_ ;
	int otherFkConstraints_ ;

	SetInfo(std::string tableName, RelationType type, const std::string& joinName, const std::string& joinSelfId, int someFkConstraints)
		: 	tableName_(tableName),
			joinName_(joinName),
			joinSelfId_(joinSelfId),
			type_(type),
			fkConstraints_(someFkConstraints)
	{}

	std::string debug(int tab)
	{
		std::stringstream ss ;
		std::string stab(tab, ' ') ;
		std::string stab1(tab+1, ' ') ;

		ss << stab << "<SetInfo>" << std::endl ;
		ss << stab1 << "tableName: " << tableName_ << std::endl ;
		ss << stab1 << "joinName: " << joinName_ << std::endl ;
		ss << stab1 << "joinSelfId: " << joinSelfId_ << std::endl ;
		ss << stab1 << "joinOtherId: " << joinOtherId_ << std::endl ;

		switch(type_)
		{
		case OneToOne:
			ss << stab1 << "type: " << "OneToOne" << std::endl ;
			break ;
		case ManyToOne:
			ss << stab1 << "type: " << "ManyToOne" << std::endl ;
			break ;
		case ManyToMany:
			ss << stab1 << "type: " << "ManyToMany" << std::endl ;
			break ;
		}

		ss << stab1 << "fkConstraints: " << fkConstraints_ << std::endl ;
		ss << stab1 << "otherFkConstraints: " << otherFkConstraints_ << std::endl ;

		return ss.str() ;
	}
};

}}

#endif
