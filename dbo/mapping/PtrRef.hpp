#ifndef _DBO_MAPPING_KEYREF_HPP_
#define _DBO_MAPPING_KEYREF_HPP_

#include <dbo/ptr.hpp>
#include <dbo/traits/dbo_traits.hpp>

namespace dbo {
namespace mapping {

template<class C>
class PtrRef
{
public:
	PtrRef(ptr<C>& value, const std::string& name, int size, int fkConstraints) ;

	const std::string& name() const
	{
		return name_ ;
	}

	int fkConstraints() const
	{
		return fkConstraints_ ;
	}

	ptr<C>& value() const
	{
		return value_ ;
	}

	typename traits::dbo_traits<C>::IdType id() const
	{
		return value_.id() ;
	}

	const std::type_info* type() const ;

	template<typename A> void visit(A& action, connection& conn) const ;

private:
	ptr<C>& value_ ;
	std::string name_ ;
	int size_ ;
	int fkConstraints_ ;
} ;

}}

#endif
