#ifndef _DBO_PTR_HPP_
#define _DBO_PTR_HPP_

#include <dbo2/traits/dbo_traits.hpp>

namespace dbo2 {

namespace action {
template <class T> class Delete ;
template <class T> class Insert ;
template <class T> class Update ;
template <class T> class LoadDb ;
}

template <class C> class ptr ;
template<class C> std::ostream& operator<<(std::ostream& o, const ptr<C>& _ptr) ;


class ptr_base
{

} ;

template<class C>
class ptr : public ptr_base
{
private:
	typedef typename boost::remove_const<C>::type MutC;
public:
	typedef typename traits::dbo_traits<C>::IdType IdType ;

	ptr() ;
	ptr(C* obj) ;

	/*! \brief Copy constructor.
	 */
	ptr(const ptr<C>& other) ;

	template<class D>
	ptr(const ptr<D>& other) ;

	virtual ~ptr() ;


	/*! \brief Resets the pointer.
	 *
	 * This is equivalent to:
	 * \code
	 * p = ptr<C>(obj);
	 * \endcode
	 */
	void reset(C* obj=nullptr) ;

	/*! \brief Assignment operator.
	 */
	ptr<C>& operator=(const ptr<C>& other) ;

	template<class D>
	ptr<C>& operator=(const ptr<D>& other) ;

	/*! \brief Dereference operator.
	 *
	 * Note that this operator returns a const copy of the referenced
	 * object. Use modify() to get a non-const reference.
	 *
	 * Since this may lazy-load the underlying database object, you
	 * should have an active transaction.
	 */
	C* operator->() const ;

	/*! \brief Returns the pointer.
	 *
	 * Note that returns a const pointer. Use modify() to get a non-const
	 * pointer.
	 *
	 * Since this may lazy-load the underlying database object, you
	 * should have an active transaction.
	 *
	 * \sa modify()
	 */
	C* get() const ;

	/*! \brief Dereference operator.
	 *
	 * Note that this operator returns a const copy of the referenced
	 * object. Use modify() to get a non-const reference.
	 *
	 * Since this may lazy-load the underlying database object, you
	 * should have an active transaction.
	 */
	C& operator*() const ;

	/*! \brief Comparison operator.
	 *
	 * Two pointers are equal if and only if they reference the same
	 * database object.
	 */
	bool operator==(const ptr<MutC>& other) const ;
	bool operator==(const ptr<const C>& other) const ;

	/*! \brief Comparison operator.
	 *
	 * Two pointers are equal if and only if they reference the same
	 * database object.
	 */
	bool operator!=(const ptr<MutC>& other) const ;
	bool operator!=(const ptr<const C>& other) const ;

	/*! \brief Comparison operator.
	 *
	 * This operator is implemented to be able to store pointers in
	 * std::set or std::map containers.
	 */
	bool operator<(const ptr<MutC>& other) const ;
	bool operator<(const ptr<const C>& other) const ;

	/*! \brief Checks for null.
	 *
	 * Returns true if the pointer is pointing to a non-null object.
	 */
	explicit operator bool() const ;

	/**
	 * An object is considered as loaded if it has a content and a valid id
	 */
	bool loaded() const ;

	/**
	 * An object is considered as orphaned if it has a content and an invalid id
	 */
	bool orphan() const ;

	const IdType& id() const ;
protected:
	struct Ptr
	{
		C* value_ ;
		size_t ref_ ;
		IdType id_ ;

		// this is the table name corresponding to C type, it is needed for serialisation
		char* tableName_ ;
	} ;

	static IdType invalidId_ ;

	Ptr* ptr_ ;

	void free() ;
	void take() ;

	void id(const IdType& value) ;

	void tableName(const char* tableName) ;

	friend class connection ;
	template <class T> friend class action::Delete ;
	template <class T> friend class action::Insert ;
	template <class T> friend class action::Update ;
	template <class T> friend class action::LoadDb ;

	friend std::ostream& operator<< <>(std::ostream& o, const ptr<C>& _ptr) ;
} ;


template<typename _Tp, typename... _Args>
inline ptr<_Tp> make_ptr(_Args&&... __args)
{
    typedef typename std::remove_const<_Tp>::type _Tp_nc ;
    return ptr<_Tp>(new _Tp_nc(std::forward<_Args>(__args)...)) ;
}

}

#endif
