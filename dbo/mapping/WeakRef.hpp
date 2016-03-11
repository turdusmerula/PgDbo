#ifndef _DBO_MAPPING_REFREF_HPP_
#define _DBO_MAPPING_REFREF_HPP_

#include <dbo/ptr.hpp>
#include <dbo/traits/dbo_traits.hpp>

namespace dbo {
namespace mapping {

template<class C>
class WeakRef
{
public:
	WeakRef(weak_ptr<C>& value, const std::string& name, int size, int fkConstraints) ;
	WeakRef(weak_ptr<C>& value, const std::string& joinname, int fkConstraints) ;

	const std::type_info* type() const ;

	template<typename A> void visit(A& action, connection& conn) const ;

	const std::string& name() const { return name_ ; }
	int fkConstraints() const {	return fkConstraints_ ;	}
	weak_ptr<C>& value() const { return value_ ; }
	typename traits::dbo_traits<C>::IdType id() const { return value_.id() ; }
	bool nameIsJoin() const { return nameIsJoin_ ; }

	std::string debug(int tab=0) const ;
private:
	weak_ptr<C>& value_ ;
	std::string name_ ;
	int size_ ;
	int fkConstraints_ ;

	bool nameIsJoin_ ;
} ;

}}

#endif
