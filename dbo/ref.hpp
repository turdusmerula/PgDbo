#ifndef _DBO_REF_HPP_
#define _DBO_REF_HPP_

namespace dbo {
template <class C> class ptr ;

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

class ref_base
{

} ;

template<class C>
class ref : public ref_base
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	ref() ;

	ref(const ptr<C>& other) ;

	/*! \brief Copy constructor.
	 */
	ref(const ref<C>& other) ;

	virtual ~ref() ;


	/*! \brief Assignment operator.
	 */
	ref<C>& operator=(const ref<C>& other) ;

	/*! \brief Assignment operator.
	 */
	ref<C>& operator=(const ptr<C>& other) ;

	template<class D>
	ref<C>& operator=(const ref<D>& other) ;

	template<class D>
	ref<C>& operator=(const ptr<D>& other) ;


	/*! \brief Comparison operator.
	 *
	 * Two pointers are equal if and only if they reference the same
	 * database object.
	 */
	bool operator==(const ref<C>& other) const ;

	/*! \brief Comparison operator.
	 *
	 * Two pointers are equal if and only if they reference the same
	 * database object.
	 */
	bool operator!=(const ref<const C>& other) const ;

	/*! \brief Comparison operator.
	 *
	 * This operator is implemented to be able to store pointers in
	 * std::set or std::map containers.
	 */
	bool operator<(const ref<C>& other) const ;

	/*! \brief Checks for id.
	 *
	 * Returns true if the reference is pointing to a valid id.
	 */
	explicit operator bool() const ;

	const IdType& id() const ;
protected:
	IdType id_ ;

	friend class connection ;
	template <class T> friend class action::Delete ;
	template <class T> friend class action::Insert ;
	template <class T> friend class action::Update ;
	template <class T> friend class action::SelectById ;
	friend class query ;

	friend std::ostream& operator<< <>(std::ostream& o, const ptr<C>& _ptr) ;
	template <class D> friend class collection ;
} ;

}

#endif
