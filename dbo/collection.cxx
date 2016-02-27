namespace dbo {

// init static invalidId_
template<class C>
typename collection<C>::IdType collection<C>::invalidId_=traits::dbo_traits<C>::invalidId() ;

template <class C>
collection<C>::collection()
{

}

template <class C>
collection<C>::collection(const collection<C>& other)
{
	for(auto& ptr : other.ptrs_)
	{
		ptrs_.push_back(ptr) ;
		take(ptr) ;
	}
}

template <class C>
collection<C>::~collection()
{
	for(auto& ptr : ptrs_)
		free(ptr) ;
}

template <class C>
collection<C>& collection<C>::operator=(const collection<C>& other)
{
	return *this ;
}

template <class C>
void collection<C>::push_back(const ptr<C>& _ptr)
{
	Ptr*& ptr=const_cast<dbo::ptr<C>&>(_ptr).ptr_ ;
	ptrs_.push_back(ptr) ;
	take(ptr) ;
}

template <class C>
void collection<C>::clear()
{
	for(auto& ptr : ptrs_)
		free(ptr) ;
	ptrs_.clear() ;
}

template <class C>
void collection<C>::free(Ptr*& ptr)
{
	if(ptr)
	{
		ptr->ref_-- ;
		if(ptr->ref_==0)
		{
			delete ptr->value_ ;
			delete ptr ;
			ptr = nullptr ;
		}
	}

}

template <class C>
void collection<C>::take(Ptr*& ptr)
{
	if(ptr)
		ptr->ref_++ ;
}

template <class C>
typename collection<C>::iterator collection<C>::begin()
{
	return collection<C>::iterator(*this) ;
}

template <class C>
typename collection<C>::iterator collection<C>::end()
{
	return collection<C>::iterator() ;
}

template <class C>
size_t collection<C>::size()
{
	return ptrs_.size() ;
}

template <class C>
bool collection<C>::empty()
{
	return ptrs_.empty() ;
}




template <class C>
collection<C>::iterator::Itr::Itr(collection<C>& coll)
	:	coll_(coll)
{

}




template <class C>
collection<C>::iterator::iterator()
	:	itr_(nullptr)
{

}

template <class C>
collection<C>::iterator::iterator(collection<C>& coll)
{
	itr_ = std::make_shared<Itr>(coll) ;
	itr_->iptr_ = coll.ptrs_.begin() ;
}

template <class C>
collection<C>::iterator::iterator(const collection<C>::iterator& other)
{
	itr_ = other.itr_ ;
}

template <class C>
collection<C>::iterator::~iterator()
{

}

template <class C>
typename collection<C>::iterator& collection<C>::iterator::operator=(const collection<C>::iterator& other)
{
	itr_ = other.itr_ ;
}

template <class C>
ptr<C> collection<C>::iterator::operator*()
{
	if(itr_)
	{
		ptr<C> res(*itr_->iptr_) ;
		return res ;
	}
	return ptr<C>() ;
}

template <class C>
ptr<C>* collection<C>::iterator::operator->()
{
	if(itr_)
	{
		ptr<C> res(*itr_->iptr_) ;
		return res ;
	}
	return ptr<C>() ;
}

template <class C>
bool collection<C>::iterator::operator==(const collection<C>::iterator& other) const
{
	return itr_==other.itr_ || end()==other.end() ;
}

template <class C>
bool collection<C>::iterator::operator!=(const collection<C>::iterator& other) const
{
	return !operator==(other) ;
}

template <class C>
typename collection<C>::iterator& collection<C>::iterator::operator++()
{
	if(itr_)
		itr_->iptr_++ ;
	return *this ;
}

template <class C>
typename collection<C>::iterator collection<C>::iterator::operator++(int value)
{
	if(itr_)
		itr_->iptr_.operator++(value) ;
	return *this ;
}

template <class C>
bool collection<C>::iterator::end() const
{
	return itr_==nullptr || itr_->iptr_==itr_->coll_.ptrs_.end() ;
}

}
