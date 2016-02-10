#ifndef _DBO_DATABASE_H_
#define _DBO_DATABASE_H_

#include <unordered_map>
#include <map>
#include <memory>
#include <set>

#include <dbo2/mapping/JoinId.h>

#include <dbo2/traits/SqlPostgresTypes.hpp>

namespace dbo2 {
namespace action {
class InitSchema ;
}

namespace mapping {
class MappingInfo ;
template <class T> class Mapping ;
class FieldInfo ;
template<class C> class KeyRef ;
}

class database
{
public:
	database() ;
	virtual ~database() ;

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

protected:

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

	void executeSql(std::vector<std::string>& sql, std::ostream *sout) ;
	void executeSql(std::stringstream& sql, std::ostream *sout) ;

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

	void prepareUpdateStatements(MappingInfoPtr mapping, bool& firstField) ;
	void prepareDeleteStatements(MappingInfoPtr mapping, bool& firstField) ;
	void prepareSelectedByIdStatements(MappingInfoPtr mapping, bool& firstField) ;
	void prepareCollectionsStatements(MappingInfoPtr mapping, bool& firstField) ;
	void prepareStatements(MappingInfoPtr mapping) ;

	template<class C> std::shared_ptr<mapping::Mapping<C>> getMapping() ;
	MappingInfoPtr getMapping(const std::string& tableName) const ;

	friend class action::InitSchema ;
	template<class C> friend class mapping::KeyRef ;

} ;

}

#endif
