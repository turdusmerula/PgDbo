namespace dbo2 {

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
	Ptr*& ptr=const_cast<dbo2::ptr<C>&>(_ptr).ptr_ ;
	ptrs_.push_back(ptr) ;
	take(ptr) ;
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

}
