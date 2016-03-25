#ifndef _DBO_MAPPING_COLLECTIONREF_HPP_
#define _DBO_MAPPING_COLLECTIONREF_HPP_

namespace dbo {
namespace mapping {

template<class C>
class CollectionRef
{
public:
	CollectionRef(collection<C>& value, RelationType type, const std::string& joinName, const std::string& joinId, int fkConstraints)
		:	value_(value),
			type_(type),
			joinName_(joinName),
			joinId_(joinId),
			fkConstraints_(fkConstraints)
	{}

	collection<C>& value() const { return value_ ; }
	const std::string& joinName() const { return joinName_ ; }
	const std::string& joinId() const { return joinId_ ; }
	RelationType type() const { return type_ ; }
	int fkConstraints() const { return fkConstraints_ ; }

	std::string debug(int tab=0) const
	{
		std::stringstream ss ;
		std::string stab(tab*2, ' ') ;
		std::string stab1((tab+1)*2, ' ') ;

		ss << stab << "<CollectionRef>" << std::endl ;
		ss << stab1 << "joinName_: " << joinName_ << std::endl ;
		ss << stab1 << "joinId_: " << joinId_ << std::endl ;
		switch(type_)
		{
		case OneToOne:
			ss << stab1 << "type_: OneToOne" << std::endl ;
			break ;
		case ManyToOne:
			ss << stab1 << "type_: ManyToOne" << std::endl ;
			break ;
		case ManyToMany:
			ss << stab1 << "type_: ManyToMany" << std::endl ;
			break ;
		}
		ss << stab1 << "fkConstraints_: " << fkConstraints_ << std::endl ;

		return ss.str() ;
	}


private:
	collection<C>& value_ ;
	std::string joinName_ ;
	std::string joinId_ ;
	RelationType type_ ;
	int fkConstraints_ ;
};

}}

#endif
