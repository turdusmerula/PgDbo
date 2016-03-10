#ifndef _DBO_ACTION_SQLUPDATE_HPP_
#define _DBO_ACTION_SQLUPDATE_HPP_

namespace dbo {

namespace action {

enum SqlUpdateState {
	PreparingValues=0,
	PreparingId,
} ;

struct SqlUpdateData {
	int params_ ;
	int idparams_ ;
	std::stringstream sql_ ;

	SqlUpdateData()
		:	params_(0),
			idparams_(0)
	{}
} ;

template<class C>
class SqlUpdate
{
public:
	using IdType = typename traits::dbo_traits<C>::IdType ;

	SqlUpdate(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt) ;

	void visit() ;

	template<typename V> void act(const mapping::FieldRef<V>& field) ;

	template<typename V> void actId(V& value, const std::string& name, int size) ;
	template<class D> void actId(ptr<D>& value, const std::string& name, int size, int fkConstraints) ;

	template <class D> void actPtr(const mapping::PtrRef<D>& field) ;
	template <class D> void actWeakPtr(const mapping::WeakRef<D>& field) ;

	template<class D> void actCollection(const mapping::CollectionRef<D>& field) ;

	connection& conn() { return stmt_.conn() ; } ;
private:
	std::shared_ptr<mapping::Mapping<C>> mapping_ ;
	stmt::PreparedStatement& stmt_ ;

	std::shared_ptr<SqlUpdateData> data_ ;

	SqlUpdateState state_ ;

	SqlUpdate(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlUpdateData> data) ;

	template <class D> friend class SqlUpdate ;
};

}}

#endif
