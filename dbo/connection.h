#ifndef _DBO_DATABASE_H_
#define _DBO_DATABASE_H_

#include <unordered_map>
#include <map>
#include <memory>
#include <set>

#include <dbo/action/ActionOption.h>
#include <dbo/mapping/JoinId.h>
#include <dbo/mapping/MappingInfo.h>
#include <dbo/traits/sql_postgres_types.hpp>
#include <dbo/traits/dbo_traits.hpp>
#include <dbo/transaction.h>
#include <dbo/query.hpp>

struct pg_conn;
typedef struct pg_conn PGconn;

namespace dbo {

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
	dbo::transaction& transaction() ;

	/**
	 * Same than transaction() with auto commit/rollback functionality.
	 * In case an error is catched then auto rollback is applied and error is rethrowed to caller
	 */
	void transaction(std::function<void()> func) ;


	/**
	 * Persists an object inside database and attribute it an id
	 */
	template<class C>
	ptr<C>& insert(ptr<C>& ptr, ActionOption opt=opt::None) ;

	/**
	 * Persists an object inside database and attribute it an id
	 */
	template<class C>
	ptr<C> insert(weak_ptr<C>& ptr, ActionOption opt=opt::None) ;

	/**
	 * bulk insert content of collection
	 */
	template<class C>
	collection<C>& bulk_insert(collection<C>& coll) ;


	/**
	 * Persists a modified object inside database
	 */
	template<class C>
	ptr<C> update(ptr<C>& ptr, ActionOption opt=opt::None) ;


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
	 * Create query from sql request
	 */
	dbo::query query(const std::string& sql) ;


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
	typedef const std::type_info* const_typeinfo_ptr ;
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

	using StatementList = std::vector<std::shared_ptr<stmt::Statement>> ;
	using TypeStatementList = std::map<const_typeinfo_ptr, StatementList, typecomp> ;

	bool schemaInitialized_ ;
	ClassRegistry classRegistry_ ;
	TableRegistry tableRegistry_ ;
	TypeStatementList statements_ ;

	dbo::transaction transaction_ ;	// current living transaction

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

	template <class C, class Statement=stmt::Statement>
	Statement& getStatement(mapping::MappingInfo::StatementType type) ;

	template <class T> friend class action::LoadId ;
	template <class T> friend class action::LoadDb ;
	template <class T> friend class action::Delete ;
	template <class T, class P> friend class action::Insert ;
	template <class T, class U> friend class action::InsertRelation ;
	template <class T, class U> friend class action::InsertCollection ;
	template <class T> friend class action::SelectById ;
	template <class T> friend class action::Update ;
	template <class T> friend class action::SqlInsert ;
	template <class T, class U> friend class action::SqlInsertRelation ;
	template <class T> friend class action::SqlSelect ;
	template <class T, class U> friend class action::SqlSelectCollection ;
	template <class T> friend class action::SqlUpdate ;
	friend class action::InitSchema ;
	template<class C> friend class mapping::PtrRef ;
	template<class C> friend class mapping::WeakRef ;
	template<class C> friend class lazy_ptr ;
	friend class stmt::PreparedStatement ;
	friend class stmt::BulkStatement ;
	friend class query ;
	friend class transaction ;


} ;

}

#endif
