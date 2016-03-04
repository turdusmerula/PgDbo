
namespace dbo {

// init static invalidId_
template<class C>
const typename ptr<C>::IdType ptr<C>::invalidId=traits::dbo_traits<C>::invalidId() ;

template <class C>
ptr<C>::ptr() noexcept
	:	tableName_(nullptr)
{
}

template <class C>
ptr<C>::ptr(std::shared_ptr<Ptr> ptr)
	:	ptr_(ptr),
		tableName_(nullptr)
{
}

template<class C>
ptr<C>::ptr(const ptr<C>& other)
	:	ptr_(other.ptr_),
		tableName_(other.tableName_)
{
}

template <class C>
template<class D>
ptr<C>::ptr(const ptr<D>& other, typename boost::detail::sp_enable_if_convertible<D, C>::type)
	:	ptr_(other.ptr_),
		tableName_(other.tableName_)
{
}

template<class C>
ptr<C>::ptr(const weak_ptr<C>& other)
	:	ptr_(other.ptr_),
		tableName_(other.tableName_)
{
	if(ptr_==nullptr && other.cache_id_!=invalidId)
		ptr_ = std::make_shared<Ptr>(C(), other.cache_id_) ;
}


template <class C>
ptr<C>::~ptr()
{
}

template<class C>
void ptr<C>::reset()
{
	ptr().swap(*this) ;
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
void ptr<C>::swap(ptr& other) noexcept
{
	ptr_.swap(other.ptr_) ;
}

template<class C>
ptr<C>& ptr<C>::operator=(const ptr<C>& other)
{
	ptr(other).swap(*this) ;

	return *this;
}

template <class C>
template<class D>
ptr<C>& ptr<C>::operator=(const ptr<D>& other)
{
	ptr(other).swap(*this) ;

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
	if(ptr_ && !(ptr_->id_==traits::dbo_traits<C>::invalidId()) && ptr_->loaded_)
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
	return invalidId ;
}

template<class C>
void ptr<C>::id(const ptr<C>::IdType& value)
{
	if(ptr_)
		ptr_->id_ = value ;
}

template<class C>
long ptr<C>::use_count() const noexcept
{
	return ptr_.use_count() ;
}

template<class C>
void ptr<C>::modify()
{
	if(ptr_)
		ptr_->modified_ = true ;
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
void ptr<C>::tableName(const char* tableName)
{
	tableName_ = const_cast<char*>(tableName) ;
}

template<class C>
std::ostream& operator<<(std::ostream& o, const dbo::ptr<C>& _ptr)
{
	if(_ptr.ptr_ && _ptr.tableName_)
		return o << "[" << _ptr.tableName_ << ": " << _ptr.id() << "]" ;
	else
		return o << "[null]" ;
}

}
