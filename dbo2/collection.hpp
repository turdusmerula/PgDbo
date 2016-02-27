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
    class iterator ;
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

    void clear() ;

	/*! \brief Returns an iterator to the begin of the %collection.
	 *
	 * \sa end()
	 */
	iterator begin() ;

	/*! \brief Returns an iterator to the end of the %collection.
	 *
	 * \sa begin()
	 */
	iterator end() ;

	size_t size() ;
	bool empty() ;

protected:
    using Ptr = typename ptr<C>::Ptr ;

    std::deque<Ptr*> ptrs_ ;

    // this is the table name corresponding to C type, it is needed for serialisation
	char* tableName_ ;

	static IdType invalidId_ ;

	void free(Ptr*& ptr) ;
	void take(Ptr*& ptr) ;
} ;


template <class C>
class collection<C>::iterator : public std::iterator<std::input_iterator_tag, C>
{
public:
	/*! \brief Copy constructor.
	 */
	iterator(const iterator& other) ;

	/*! \brief Destructor.
	 */
	~iterator() ;

	/*! \brief Assignment operator.
	 */
	iterator& operator=(const iterator& other) ;

	/*! \brief Dereference operator.
	 */
	ptr<C> operator*() ;

	/*! \brief Dereference operator.
	 */
	ptr<C> *operator->() ;

	/*! \brief Comparison operator.
	 *
	 * Returns true if two iterators point to the same value in the
	 * same %collection, or point both to the end of a collection.
	 */
	bool operator==(const iterator& other) const ;

	/*! \brief Comparison operator.
	 */
	bool operator!=(const iterator& other) const ;

	/*! \brief Pre increment operator.
	 */
	iterator& operator++() ;

	/*! \brief Post increment operator.
	 */
	iterator operator++(int) ;

private:
	struct Itr
	{
		collection<C>& coll_ ;
		typename std::deque<Ptr*>::iterator iptr_ ;
		Itr(collection<C>& coll) ;
	} ;

	std::shared_ptr<Itr> itr_ ;

	bool end() const ;

	iterator();
	iterator(collection<C>& collection) ;

	void takeImpl();
	void releaseImpl();

	friend class collection<C> ;
//	friend class const_iterator;
};

}

#endif
