namespace dbo2 {
namespace action {

template<class C, class Enable>
template<class A>
void persist<C, Enable>::apply(C& obj, A& action)
{
	obj.persist(action) ;
}

}}
