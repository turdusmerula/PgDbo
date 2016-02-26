#ifndef _DBO_DATABASE_H_
#define _DBO_DATABASE_H_

#include <unordered_map>
#include <map>
#include <memory>
#include <set>

#include <dbo2/mapping/JoinId.h>
#include <dbo2/traits/SqlPostgresTypes.hpp>
#include <dbo2/traits/dbo_traits.hpp>
#include <dbo2/transaction.h>
#include <dbo2/query.hpp>

#include <pqxx/pqxx>

struct pg_conn;
typedef struct pg_conn PGconn;

namespace dbo2 {
template <class T> class collection ;
template <class T> class ptr ;
class query ;

namespace action {
template <class T> class Delete ;
template <class T> class Insert ;
template <class T> class SelectById ;
template <class T> class Update ;
class InitSchema ;
}

namespace mapping {
class MappingInfo ;
template <class T> class Mapping ;
class FieldInfo ;
template<class C> class PtrRef ;
}

namespace stmt {
class BulkStatement ;
class PreparedStatement ;
}

class connection
{
public:
	connection() ;

	virtual ~connection() ;

	/*! \brief Connects to database.	 *
	 */
	void connect(std::string options="") ;

	bool connected() const ;

	/*! \brief Maps a class to a database table.
	 *
	 * The class \p C is mapped to table with \p tableName. You
	 * need to map classes to tables.
	 *
	 * You may provide a schema-qualified table name, if the underlying
	 * database supports this, eg. <tt>"myschema.users"</tt>.
	 */
	template<class C>
	void mapClass(std::string name) ;

	/*! \brief Returns the mapped table name for a class.
	 *
	 * \sa mapClass(), tableNameQuoted()
	 */
	template<class C>
	const std::string& tableName() const ;

	/*! \brief Creates the database schema.
	 *
	 * This will create the database schema of the mapped tables. Schema
	 * creation will fail if one or more tables already existed. The creation
	 * of the tables is executed in a transaction that is rolled back when
	 * an error occurs.
	 *
	 * This method throws an dbo::Exception if the table creation failed.
	 *
	 * \sa mapClass(), dropTables()
	 */
	void createTables() ;

	/*! \brief Returns database creation SQL.
	 */
	std::string tableCreationSql() ;


	/**
	 * Open a new transaction for the session, if transaction already exists then access it
	 */
	dbo2::transaction& transaction() ;

	/**
	 * Same than transaction() with auto commit/rollback functionality.
	 * In case an error is catched then auto rollback is applied and error is rethrowed to caller
	 */
	void transaction(std::function<void()> func) ;


	/**
	 * Persists an object inside database and attribute it an id
	 */
	template<class C>
	ptr<C>& insert(ptr<C>& ptr) ;

	/**
	 * bulk insert content of collection
	 */
	template<class C>
	collection<C>& insert(collection<C>& coll) ;

	/**
	 * Persists a modified object inside database
	 */
	template<class C>
	ptr<C> update(ptr<C>& ptr) ;


	// TODO: implement load lazy and load recursive (?)
	/**
	 * Reload an existing object from database
	 */
	template<class C>
	ptr<C> load(ptr<C>& ptr) ;

	/**
	 * Load an object from database
	 */
	template<class C>
	ptr<C> load(const typename traits::dbo_traits<C>::IdType& id) ;


	/**
	 * Remove an existing object from database
	 */
	template<class C>
	void remove(ptr<C>& ptr) ;

	/**
	 * Create a select query from mapping
	 */
	template<class C>
	query find(const std::string& condition="") ;

	/**
	 * Create a select query from collection mapping
	 */
	template<class C>
	query find(const collection<C>& collection, const std::string& condition="") ;

	/**
	 * Create query from sql request
	 */
	dbo2::query query(const std::string& sql) ;


	void debug() ;
	bool showQueries() const { return showQueries_ ; }
	void showQueries(bool value) { showQueries_ = value ; }
	bool showBindings() const { return showBindings_ ; }
	void showBindings(bool value) { showBindings_ = value ; }
	bool showResults() const { return showResults_ ; }
	void showResults(bool value) { showResults_ = value ; }
protected:
	std::string options_ ;
	PGconn* conn_ ;

	// debugging flags
	bool showQueries_ ;
	bool showBindings_ ;
	bool showResults_ ;

	// RTTI class info
	typedef const std::type_info * const_typeinfo_ptr ;
	struct typecomp
	{
		bool operator()(const const_typeinfo_ptr& lhs, const const_typeinfo_ptr& rhs) const
		{
			return lhs->before(*rhs)!=0 ;
		}
	} ;

	using MappingInfoPtr = std::shared_ptr<mapping::MappingInfo> ;
	using ClassRegistry = std::map<const_typeinfo_ptr, MappingInfoPtr, typecomp> ;
	using TableRegistry = std::unordered_map<std::string, MappingInfoPtr> ;
	using Types = traits::SqlPostgresTypes ;

	bool schemaInitialized_ ;
	ClassRegistry classRegistry_ ;
	TableRegistry tableRegistry_ ;

	dbo2::transaction transaction_ ;	// current living transaction

	void executeSql(const std::vector<std::string>& sql, std::ostream *sout=nullptr) ;
	void executeSql(const std::stringstream& sql, std::ostream *sout=nullptr) ;
	void executeSql(const std::string& sql, std::ostream *sout=nullptr) ;

	void resolveJoinIds(MappingInfoPtr mapping) ;
	void createRelations(MappingInfoPtr mapping, std::set<std::string>& tablesCreated, std::ostream *sout) ;

	void initSchema() ;

	void createTable(MappingInfoPtr mapping, std::set<std::string>& tablesCreated, std::ostream *sout, bool createConstraints) ;
	std::string constraintString(MappingInfoPtr mapping, const mapping::FieldInfo& field, unsigned fromIndex, unsigned toIndex) ;
	unsigned findLastForeignKeyField(MappingInfoPtr mapping, const mapping::FieldInfo& field, unsigned index) ;
	void createJoinTable(const std::string& joinName, MappingInfoPtr mapping1, MappingInfoPtr mapping2, const std::string& joinId1, const std::string& joinId2, int fkConstraints1, int fkConstraints2, std::set<std::string>& tablesCreated, std::ostream *sout) ;
	std::vector<mapping::JoinId> getJoinIds(MappingInfoPtr mapping, const std::string& joinId) ;
	void addJoinTableFields(MappingInfoPtr result, MappingInfoPtr mapping, const std::string& joinId, const std::string& keyName, int fkConstraints) ;
	void createJoinIndex(MappingInfoPtr joinTableMapping, MappingInfoPtr mapping, const std::string& joinId, const std::string& foreignKeyName, std::ostream *sout) ;

	void prepareInsertStatements(MappingInfoPtr mapping) ;
	void prepareUpdateStatements(MappingInfoPtr mapping) ;
	void prepareDeleteStatements(MappingInfoPtr mapping) ;
	void prepareSelectedByIdStatements(MappingInfoPtr mapping) ;
	void prepareCollectionsStatements(MappingInfoPtr mapping) ;
	void prepareStatements(MappingInfoPtr mapping) ;

	template<class C> std::shared_ptr<mapping::Mapping<C>> getMapping() ;
	MappingInfoPtr getMapping(const std::string& tableName) const ;

	template <class T> friend class action::Delete ;
	template <class T> friend class action::Insert ;
	template <class T> friend class action::SelectById ;
	template <class T> friend class action::Update ;
	friend class action::InitSchema ;
	template<class C> friend class mapping::PtrRef ;
	friend class stmt::PreparedStatement ;
	friend class stmt::BulkStatement ;
	friend class query ;
	friend class transaction ;


} ;

}

#endif
