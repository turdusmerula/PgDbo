#ifndef _DBO_COLLECTION_HPP_
#define _DBO_COLLECTION_HPP_

#include <deque>

//#include <dbo2/traits/dbo_traits.hpp>

namespace dbo2 {
template <class C> class ptr ;

template <class C>
class collection
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	/*! \brief Default constructor.
     *
     * Constructs an empty %collection that is not bound to a table or query
     */
	collection() ;

    /*! \brief Copy constructor.
     */
    collection(const collection<C>& other) ;

    /*! \brief Destructor.
     */
    ~collection();


    /*! \brief Assignment operator.
     */
    collection<C>& operator=(const collection<C>& other) ;

    void push_back(const ptr<C>& ptr) ;

protected:
    using Ptr = typename ptr<C>::Ptr ;

    std::deque<Ptr*> ptrs_ ;

    // this is the table name corresponding to C type, it is needed for serialisation
	char* tableName_ ;

	static IdType invalidId_ ;

	void free(Ptr*& ptr) ;
	void take(Ptr*& ptr) ;
} ;

}

#endif
