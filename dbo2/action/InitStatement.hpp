#ifndef _DBO_ACTION_INITSTATEMENT_HPP_
#define _DBO_ACTION_INITSTATEMENT_HPP_

namespace dbo2 {
namespace mapping {
template <class T> class Mapping ;
template <class T> class FieldRef ;
}

namespace action {

/**
 * Initialize prepared statements
 */
template<class C>
class InitStatement
{
public:
	InitStatement(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;
	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::Statement stmt_ ;
};

}}

#endif
