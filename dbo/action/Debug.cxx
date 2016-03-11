namespace dbo {
namespace action {


template<class C>
Debug<C>::Debug(ptr<C> _ptr, connection& conn)
	: 	ptr_(_ptr),
		is_ptr_(true),
		tab_(0),
		conn_(conn)
{
}

template<class C>
void Debug<C>::visit()
{
	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;

	std::cout << stab << "+ visit +" << std::endl ;
	if(is_ptr_)
	{
		if(ptr_)
			ptr_->persist(*this) ;
	}
	tab_-- ;
}

template<class C>
template<typename V>
void Debug<C>::act(const mapping::FieldRef<V>& field)
{
	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;

	std::cout << stab << "+ act +" << std::endl ;
	std::cout << field.debug(tab_+1) ;
	tab_-- ;
}

template<class C>
template<typename V>
void Debug<C>::actId(V& value, const std::string& name, int size)
{
	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;
	std::string stab1((tab_+1)*2, ' ') ;

	std::cout << stab << "+ actId +" << std::endl ;
	std::cout << stab1 << "name: " << name << std::endl ;
	std::cout << stab1 << "value: " << value << std::endl ;

	// add id fields to statement
	field(*this, value, name) ;
	tab_-- ;
}

template<class C>
template<class D>
void Debug<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;
	std::string stab1((tab_+1)*2, ' ') ;

	std::cout << stab << "+ actPtr +" << std::endl ;

	// this action is C type, we need D, so we create a special one for this type
	Debug<D> action(field.value(), conn()) ;
	action.tab_ = tab_ ;

	std::cout << field.debug(tab_+1) ;

	// add id fields to statement
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

	action.visit() ;

	tab_-- ;
}

template<class C>
template<class D>
void Debug<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;
	std::string stab1((tab_+1)*2, ' ') ;

	std::cout << stab << "+ actWeakPtr +" << std::endl ;

	ptr<D> ptr ;

	// this action is C type, we need D, so we create a special one for this type
	Debug<D> action(ptr, conn()) ;
	action.tab_ = tab_ ;

	std::cout << field.debug(tab_+1) ;

	// add id fields to statement
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

	tab_-- ;
}

template<class C>
template<class D>
void Debug<C>::actCollection(const mapping::CollectionRef<D>& field)
{
	tab_++ ;
	std::stringstream ss ;
	std::string stab(tab_*2, ' ') ;
	std::string stab1((tab_+1)*2, ' ') ;

	std::cout << stab << "+ actCollection +" << std::endl ;

	for(auto ptr : field.value())
	{
		action::Debug<D> action(ptr) ;
		action.visit() ;
	}

	tab_-- ;
}

}}
