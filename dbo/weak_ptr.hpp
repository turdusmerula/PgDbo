#ifndef _DBO_REF_HPP_
#define _DBO_REF_HPP_

namespace dbo {

template<class C> std::ostream& operator<<(std::ostream& o, const ptr<C>& _ptr) ;

class weak_ptr_base
{

} ;

template<class C>
class weak_ptr : public weak_ptr_base
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	weak_ptr() noexcept ;

	/*! \brief Copy constructor.
	 */
	weak_ptr(const weak_ptr<C>& other) ;

	template<class D>
	weak_ptr(weak_ptr<D> const& other, typename boost::detail::sp_enable_if_convertible<D, C>::type=boost::detail::sp_empty()) ;


	template<class D>
	weak_ptr(ptr<D> const& other, typename boost::detail::sp_enable_if_convertible<D, C>::type=boost::detail::sp_empty()) ;

	weak_ptr(const ptr<C>& other) ;


	virtual ~weak_ptr() ;


	/*! \brief Assignment operator.
	 */
	weak_ptr<C>& operator=(const weak_ptr<C>& other) ;

	template<class D>
	weak_ptr<C>& operator=(const weak_ptr<D>& other) ;

	/*! \brief Assignment operator.
	 */
	weak_ptr<C>& operator=(const ptr<C>& other) ;

	template<class D>
	weak_ptr<C>& operator=(const ptr<D>& other) ;


	void swap(weak_ptr& other) noexcept ;


	ptr<C> lock() ;

	const IdType& id() const ;

	bool expired() const noexcept ;

protected:
	mutable IdType cache_id_ ;	// this id exists to allow the object to have an id even if not observing a ptr

	std::weak_ptr<typename ptr<C>::Ptr> ptr_ ;


	void id(const IdType& value) ;


	friend class connection ;
	template <class T> friend class action::Delete ;
	template <class T, class U> friend class action::Insert ;
	template <class T> friend class action::Update ;
	template <class T> friend class action::SelectById ;
	friend class query ;
	template <class D> friend class collection ;

	friend std::ostream& operator<< <>(std::ostream& o, const ptr<C>& _ptr) ;
	template <class D> friend class ptr ;
} ;

}

#endif
