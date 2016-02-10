#ifndef _DBO_MAPPING_KEYREF_HPP_
#define _DBO_MAPPING_KEYREF_HPP_

#include <dbo2/key.hpp>
#include <dbo2/traits/dbo_traits.hpp>

namespace dbo2 {
namespace mapping {

template<class C>
class KeyRef
{
public:
	KeyRef(key<C>& key, const std::string& name, int size, int fkConstraints) ;

	const std::string& name() const
	{
		return name_ ;
	}

	int fkConstraints() const
	{
		return fkConstraints_ ;
	}

	key<C>& value() const
	{
		return key_ ;
	}

	typename traits::dbo_traits<C>::IdType id() const
	{
		return key_.id() ;
	}

	const std::type_info* type() const ;

	/*
	 * If session = 0, the visited foreign key fields will not be named
	 * correctly (ok when e.g. reading/writing data)
	 */
	template<typename A> void visit(A& action, database& conn) const ;

private:
	key<C>& key_ ;
	std::string name_ ;
	int size_ ;
	int fkConstraints_ ;
} ;

}}

#endif
