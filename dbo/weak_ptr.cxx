namespace dbo {

template <class C>
weak_ptr<C>::weak_ptr() noexcept
	:	cache_id_(traits::dbo_traits<C>::invalidId())
{
}

template <class C>
weak_ptr<C>::weak_ptr(const weak_ptr<C>& other)
	:	ptr_(other.ptr_),
		cache_id_(other.cache_id_)
{
}

template <class C>
template<class D>
weak_ptr<C>::weak_ptr(weak_ptr<D> const& other, typename boost::detail::sp_enable_if_convertible<D, C>::type)
	:	ptr_(other.ptr_),
		cache_id_(other.cache_id_)
{
}

template <class C>
template<class D>
weak_ptr<C>::weak_ptr(ptr<D> const& other, typename boost::detail::sp_enable_if_convertible<D, C>::type)
	:	ptr_(other.ptr_)
{
	if(ptr_.expired())
		cache_id_ = traits::dbo_traits<C>::invalidId() ;
	else
		cache_id_ = other.ptr_.lock()->id_ ;
}

template <class C>
weak_ptr<C>::weak_ptr(const ptr<C>& other)
	:	ptr_(other.ptr_)
{
	if(ptr_.expired())
		cache_id_ = traits::dbo_traits<C>::invalidId() ;
	else
		cache_id_ = other.ptr_->id_ ;
}

template <class C>
weak_ptr<C>::~weak_ptr()
{
}

template <class C>
weak_ptr<C>& weak_ptr<C>::operator=(const weak_ptr<C>& other)
{
	weak_ptr(other).swap(*this) ;

	return *this;
}

template<class C>
template<class D>
weak_ptr<C>& weak_ptr<C>::operator=(const weak_ptr<D>& other)
{
	weak_ptr(other).swap(*this) ;

	return *this;
}

template <class C>
weak_ptr<C>& weak_ptr<C>::operator=(const ptr<C>& other)
{
	weak_ptr(other).swap(*this) ;

	return *this;
}

template<class C>
template<class D>
weak_ptr<C>& weak_ptr<C>::operator=(const ptr<D>& other)
{
	weak_ptr(other).swap(*this) ;

	return *this;
}

template<class C>
void weak_ptr<C>::swap(weak_ptr& other) noexcept
{
	ptr_.swap(other.ptr_) ;
}

template<class C>
const typename weak_ptr<C>::IdType& weak_ptr<C>::id() const
{
	if(ptr_.expired()==false)
		cache_id_ = ptr_.lock()->id_ ;
	return cache_id_ ;
}

template<class C>
void weak_ptr<C>::id(const weak_ptr<C>::IdType& value)
{
	cache_id_ = value ;
	if(ptr_.expired()==false)
		ptr_.lock()->id_ = value ;
}

template<class C>
bool weak_ptr<C>::expired() const noexcept
{
	return ptr_.expired() ;
}

template<class C>
ptr<C> weak_ptr<C>::lock()
{
	return ptr<C>(*this) ;
}

template<class C>
std::ostream& operator<<(std::ostream& o, const dbo::weak_ptr<C>& _ptr)
{
	if(_ptr.ptr_ && _ptr.tableName_)
		return o << "[" << _ptr.tableName_ << ": " << _ptr.id() << "]" ;
	else
		return o << "[null]" ;
}

}
