// ----------------------------------------------------------------------------
// Debug function to test lSimpleTable content
class lSimpleTable ;

template <class Q>
typename std::enable_if<std::is_same<Q, lSimpleTable>::value, bool>::type
debug_lSimpleTable(dbo::ptr<Q> p, int n)
{
	std::cout << "debug " << n << " object is null: " << " " << (p->object==nullptr?"true":"false") << std::endl ;
	return true ;
} ;

template <class Q>
typename std::enable_if<!std::is_same<Q, lSimpleTable>::value, bool>::type
debug_lSimpleTable(dbo::ptr<Q> p, int n)
{
	return false ;
} ;
// ----------------------------------------------------------------------------
