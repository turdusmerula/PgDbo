#include <boost/type_traits/remove_const.hpp>

#include <dbo2/action/InitSchema.hpp>

namespace dbo2 {
namespace mapping {

template<class C>
Mapping<C>::Mapping()
{

}

template<class C>
Mapping<C>::~Mapping()
{
//	for(typename Registry::iterator i=registry_.begin() ; i!=registry_.end() ; ++i)
//	{
//		i->second->setState(MetaDboBase::Orphaned) ;
//	}
}

template<class C>
void Mapping<C>::init()
{
	typedef typename boost::remove_const<C>::type MutC ;

	if(!initialized_)
	{
		initialized_ = true ;

		action::InitSchema action(*this) ;
		MutC dummy ;
		action.visit(dummy) ;
	}
}

}}
