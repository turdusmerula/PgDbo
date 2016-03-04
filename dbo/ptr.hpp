#ifndef _DBO_PTR_HPP_
#define _DBO_PTR_HPP_

#include <mutex>
#include <atomic>

namespace dbo {
template <class T> class weak_ptr ;

namespace action {
template <class T> class Delete ;
template <class T> class Insert ;
template <class T> class Update ;
template <class T> class SelectById ;
}

class query ;
template <class C> class collection ;
template <class C> class ptr ;
template<class C> std::ostream& operator<<(std::ostream& o, const ptr<C>& _ptr) ;


class ptr_base
{

} ;

// TODO: make ptr counter increment/decrement atomic
template<class C>
class ptr : public ptr_base
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	ptr() noexcept ;

	/*! \brief Copy constructor.
	 */
	template<class D>
	ptr(ptr<D> const& other, typename boost::detail::sp_enable_if_convertible<D, C>::type=boost::detail::sp_empty()) ;

	ptr(const ptr& other) ;

	ptr(const weak_ptr<C>& other) ;

	~ptr() ;


	/*! \brief Assignment operator.
	 */
	ptr& operator=(const ptr& other) ;

	template<class D>
	ptr<C>& operator=(const ptr<D>& other) ;

	/*! \brief Resets the pointer.
	 *
	 * This is equivalent to:
	 * \code
	 * p = ptr<C>(obj);
	 * \endcode
	 */
	void reset() ;


	/*! \brief Returns the pointer.
	 */
	C* get() const noexcept ;

    /*! \brief Dereference operator.
	 */
	C& operator*() const ;

	/*! \brief Dereference operator.
	 */
	C* operator->() const ;


    void swap(ptr& other) noexcept ;


	/*! \brief Checks for null.
	 *
	 * Returns true if the pointer is pointing to a non-null object.
	 */
	explicit operator bool() const ;

	/**
	 * An object is considered as loaded if it has a content and a valid id
	 */
    void load() ;
	bool loaded() const ;

	/**
	 * An object is considered as orphaned if it has a content and an invalid id
	 */
	bool orphan() const ;

    void modify() ;
    bool modified() const ;

    const IdType& id() const ;

    long use_count() const noexcept ;

	static const IdType invalidId ;
protected:
	struct Ptr
	{
		C value_ ;
		IdType id_ ;

		bool modified_ ;
		bool loaded_ ;

		Ptr(C&& value=C(), const typename traits::dbo_traits<C>::IdType& id=traits::dbo_traits<C>::invalidId())
			:	value_(std::move(value)),
				id_(id),
				modified_(false),
				loaded_(false)
		{}
	} ;

	std::shared_ptr<Ptr> ptr_ ;

	// this is the table name corresponding to C type, it is needed for serialisation
	char* tableName_ ;



	/**
	 * Provided for make_ptr, takes the ownership of ptr
	 */
	ptr(std::shared_ptr<Ptr> ptr) ;

	void id(const IdType& value) ;

	void tableName(const char* tableName) ;


	friend class connection ;
	template <class T> friend class action::Delete ;
	template <class T> friend class action::Insert ;
	template <class T> friend class action::Update ;
	template <class T> friend class action::SelectById ;
	friend class query ;
	template <class D> friend class collection ;
	template <class D> friend class weak_ptr ;

	template<typename _Tp, typename... _Args> friend inline ptr<_Tp> make_ptr(_Args&&... __args) ;
	friend std::ostream& operator<< <>(std::ostream& o, const ptr<C>& _ptr) ;
} ;


template<typename _Tp, typename... _Args>
inline ptr<_Tp> make_ptr(_Args&&... __args)
{
    typedef typename std::remove_const<_Tp>::type _Tp_nc ;
    return ptr<_Tp>(
    	std::make_shared<typename ptr<_Tp>::Ptr>(_Tp(_Tp_nc(std::forward<_Args>(__args)...)))
	);
}


template<typename _Tp1, typename _Tp2>
inline bool operator==(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{ return __a.get() == __b.get(); }

template<typename _Tp>
inline bool operator==(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return !__a; }

template<typename _Tp>
inline bool operator==(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return !__a; }

template<typename _Tp1, typename _Tp2>
inline bool operator!=(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{ return __a.get() != __b.get(); }

template<typename _Tp>
inline bool operator!=(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return (bool)__a; }

template<typename _Tp>
inline bool operator!=(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return (bool)__a; }

template<typename _Tp1, typename _Tp2>
inline bool operator<(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{
	typedef typename std::common_type<_Tp1*, _Tp2*>::type _CT;
    return std::less<_CT>()(__a.get(), __b.get());
}

template<typename _Tp>
inline bool operator<(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return std::less<_Tp*>()(__a.get(), nullptr); }

template<typename _Tp>
inline bool operator<(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return std::less<_Tp*>()(nullptr, __a.get()); }

template<typename _Tp1, typename _Tp2>
inline bool operator<=(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{ return !(__b < __a); }

template<typename _Tp>
inline bool operator<=(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return !(nullptr < __a); }

template<typename _Tp>
inline bool operator<=(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return !(__a < nullptr); }

template<typename _Tp1, typename _Tp2>
inline bool operator>(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{ return (__b < __a); }

template<typename _Tp>
inline bool operator>(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return std::less<_Tp*>()(nullptr, __a.get()); }

template<typename _Tp>
inline bool operator>(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return std::less<_Tp*>()(__a.get(), nullptr); }

template<typename _Tp1, typename _Tp2>
inline bool operator>=(const ptr<_Tp1>& __a, const ptr<_Tp2>& __b) noexcept
{ return !(__a < __b); }

template<typename _Tp>
inline bool operator>=(const ptr<_Tp>& __a, std::nullptr_t) noexcept
{ return !(__a < nullptr); }

template<typename _Tp>
inline bool operator>=(std::nullptr_t, const ptr<_Tp>& __a) noexcept
{ return !(nullptr < __a); }

}

#endif
