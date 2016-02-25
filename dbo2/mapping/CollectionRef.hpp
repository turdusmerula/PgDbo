#ifndef _DBO_MAPPING_COLLECTIONREF_HPP_
#define _DBO_MAPPING_COLLECTIONREF_HPP_

namespace dbo2 {
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
private:
	collection<C>& value_ ;
	std::string joinName_ ;
	std::string joinId_ ;
	RelationType type_ ;
	int fkConstraints_ ;
};

}}

#endif
