namespace dbo {

template <class C>
ref<C>::ref()
	:	id_(traits::dbo_traits<C>::invalidId())
{
}

template <class C>
ref<C>::ref(const ptr<C>& other)
	:	id_(other.id())
{
}

template <class C>
ref<C>::ref(const ref<C>& other)
	:	id_(other.id())
{
}

template <class C>
ref<C>::~ref()
{
}

template <class C>
ref<C>& ref<C>::operator=(const ref<C>& other)
{
	id_ = other.id() ;
	return *this ;
}

template <class C>
ref<C>& ref<C>::operator=(const ptr<C>& other)
{
	id_ = other.id() ;
	return *this ;
}

template<class C>
template<class D>
ref<C>& ref<C>::operator=(const ref<D>& other)
{
	// Check if we can convert D* to C*
	D d ;
	dynamic_cast<C*>(&d) ;

	id_ = other.id() ;

	return *this ;
}

template<class C>
template<class D>
ref<C>& ref<C>::operator=(const ptr<D>& other)
{
	// Check if we can convert D* to C*
	D d ;
	dynamic_cast<C*>(&d) ;

	id_ = other.id() ;

	return *this ;
}

template<class C>
bool ref<C>::operator==(const ref<C>& other) const
{
	return id_==other.id_ ;
}

template<class C>
bool ref<C>::operator!=(const ref<const C>& other) const
{
	return !operator=(other) ;
}

template<class C>
bool ref<C>::operator<(const ref<C>& other) const
{
	return id_<other.id_ ;
}

template<class C>
ref<C>::operator bool() const
{
	return id_!=traits::dbo_traits<C>::invalidId() ;
}

template<class C>
const typename ref<C>::IdType& ref<C>::id() const
{
	return id_ ;
}

}
