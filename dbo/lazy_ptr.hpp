#ifndef _DBO_LAZY_PTR_H_
#define _DBO_LAZY_PTR_H_

#include <dbo/connection.h>
#include <dbo/ptr.hpp>

namespace dbo {

template <class C> class collection ;

template<class C> class ptr ;

template<class C>
class lazy_ptr : public ptr_base
{
public:
	lazy_ptr(ptr<C> ptr, connection& conn) ;

	/*! \brief Dereference operator.
	 */
	C* operator->() const ;

	template<class D>
	collection<D>& load(collection<D>& coll) ;

protected:
	connection& conn_ ;
	ptr<C> ptr_ ;
} ;

}

#endif
