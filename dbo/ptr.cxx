
namespace dbo {

// init static invalidId_
template<class C>
const typename ptr<C>::IdType ptr<C>::invalidId=traits::dbo_traits<C>::invalidId() ;

template <class C>
ptr<C>::ptr() noexcept
	:	cache_id_(traits::dbo_traits<C>::invalidId())
{
}

template <class C>
ptr<C>::ptr(std::shared_ptr<Ptr> ptr)
	:	ptr_(ptr),
		cache_id_(ptr_->id_)
{
}

template<class C>
ptr<C>::ptr(const ptr<C>& other)
	:	ptr_(other.ptr_),
		cache_id_(other.cache_id_)
{
}

template <class C>
template<class D>
ptr<C>::ptr(const ptr<D>& other, typename boost::detail::sp_enable_if_convertible<D, C>::type)
	:	ptr_(other.ptr_),
		cache_id_(other.cache_id_)
{
}

template<class C>
ptr<C>::ptr(const weak_ptr<C>& other)
	:	ptr_(other.ptr_),
		cache_id_(other.cache_id_)
{
	if(ptr_==nullptr && !(other.cache_id_==invalidId))
		ptr_ = std::make_shared<Ptr>(C(), other.cache_id_) ;
}


template <class C>
ptr<C>::~ptr()
{
}

template<class C>
void ptr<C>::reset()
{
	cache_id_ = invalidId ;
	ptr_.reset() ;
}

template<class C>
C* ptr<C>::get() const noexcept
{
	if(ptr_)
		return &(ptr_.get()->value_) ;
	return nullptr ;
}

template<class C>
C& ptr<C>::operator*() const
{
    BOOST_ASSERT( ptr_ != nullptr ) ;
	return ptr_->value_ ;
}

template<class C>
C* ptr<C>::operator->() const
{
    BOOST_ASSERT( ptr_ != nullptr ) ;
	return &(ptr_->value_) ;
}

template<class C>
lazy_ptr<C> ptr<C>::operator()(connection& conn) const
{
	return lazy_ptr<C>(const_cast<ptr<C>&>(*this), conn) ;
}

template<class C>
void ptr<C>::swap(ptr& other) noexcept
{
	cache_id_ = other.cache_id_ ;
	ptr_.swap(other.ptr_) ;
}

template<class C>
ptr<C>& ptr<C>::operator=(const ptr<C>& other)
{
	cache_id_ = other.cache_id_ ;
	ptr_ = other.ptr_ ;

	return *this;
}

template<class C>
ptr<C>& ptr<C>::operator=(std::nullptr_t)
{
	reset() ;

	return *this;
}

template <class C>
template<class D>
ptr<C>& ptr<C>::operator=(const ptr<D>& other)
{
	cache_id_ = other.cache_id_ ;
	ptr_ = other.ptr_ ;

	return *this;
}

template<class C>
ptr<C>::operator bool() const
{
	return (bool)ptr_ ;
}

template<class C>
bool ptr<C>::loaded() const
{
	if(ptr_ && ptr_->loaded_ && !(ptr_->id_==traits::dbo_traits<C>::invalidId()) )
		return true ;
	return false ;
}

template<class C>
bool ptr<C>::orphan() const
{
	if(ptr_ && ptr_->id_==traits::dbo_traits<C>::invalidId())
		return true ;
	return false ;
}


template<class C>
const typename ptr<C>::IdType& ptr<C>::id() const
{
	if(ptr_)
		return ptr_->id_ ;
	return cache_id_ ;
}

template<class C>
void ptr<C>::id(const ptr<C>::IdType& value)
{
	cache_id_ = value ;
	if(ptr_)
		ptr_->id_ = value ;
}

template<class C>
long ptr<C>::use_count() const noexcept
{
	return ptr_.use_count() ;
}

template<class C>
void ptr<C>::modify(bool value)
{
	if(ptr_)
		ptr_->modified_ = value ;
}

template<class C>
bool ptr<C>::modified() const
{
	if(ptr_)
		return ptr_->modified_ ;
	return false ;
}

template<class C>
void ptr<C>::load()
{
	if(ptr_)
		ptr_->loaded_ = true ;
}

template<class C>
std::ostream& operator<<(std::ostream& o, const dbo::ptr<C>& _ptr)
{
	if(_ptr.ptr_)
		return o << "[" << _ptr.id() << "]" ;
	else
		return o << "[null]" ;
}

}
