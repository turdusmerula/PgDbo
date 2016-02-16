#include <dbo2/connection.h>

#include <dbo2/mapping/Field.h>
#include <dbo2/mapping/FieldInfo.h>
#include <dbo2/mapping/SetInfo.h>
#include <dbo2/mapping/MappingInfo.h>
#include <dbo2/mapping/Mapping.hpp>

#include <dbo2/stmt/Statement.h>

#include <dbo2/traits/sql_value_traits.hpp>
#include <dbo2/traits/StdSqlTraits.h>

#include <postgresql/libpq-fe.h>

using namespace dbo2 ;


connection::connection()
	:	schemaInitialized_(false),
		transaction_(*this),
		conn_(nullptr),
		showQueries_(false),
		showBindings_(false)
{

}

connection::~connection()
{
}

std::string& replace(std::string& s, char c, const std::string& r)
{
	std::string::size_type p=0 ;

	while((p=s.find(c, p))!=std::string::npos)
	{
		s.replace(p, 1, r) ;
		p += r.length() ;
	}

	return s ;
}

std::string quoteSchemaDot(const std::string& table)
{
	std::string result=table ;
	replace(result, '.', "\".\"") ;
	return result ;
}

void connection::connect(std::string options)
{
	options_ = options ;
	conn_ = PQconnectdb(options.c_str()) ;

	if(PQstatus(conn_)!=CONNECTION_OK)
	{
		std::string error = PQerrorMessage(conn_) ;
		PQfinish(conn_) ;
		conn_ = nullptr ;
		throw Exception("Could not connect to: "+error) ;
	}

	PQsetClientEncoding(conn_, "UTF8") ;
}

bool connection::connected() const
{
	return conn_!=nullptr ;
}

void connection::executeSql(const std::vector<std::string>& sql, std::ostream* sout)
{
	for(auto& request : sql)
		executeSql(request, sout) ;
}

void connection::executeSql(const std::stringstream& sql, std::ostream* sout)
{
	executeSql(sql.str(), sout) ;
}

void connection::executeSql(const std::string& sql, std::ostream* sout)
{
	if(sout)
		*sout << sql << ";\n" ;
	else
	{
		PGresult *result ;
		int err ;

		if(showQueries_)
			std::cerr << sql << std::endl ;

		result = PQexec(conn_, sql.c_str()) ;
		err = PQresultStatus(result) ;
		if(err!=PGRES_COMMAND_OK && err!=PGRES_TUPLES_OK)
		{
			PQclear(result) ;
			throw Exception(PQerrorMessage(conn_)) ;
		}
		PQclear(result) ;
	}
}

void connection::createTable(MappingInfoPtr mapping, std::set<std::string>& tablesCreated, std::ostream *sout, bool createConstraints)
{
	if(tablesCreated.count(mapping->tableName)!=0)
		return ;

	tablesCreated.insert(mapping->tableName) ;

	std::stringstream sql ;

	sql << "create table \"" << quoteSchemaDot(mapping->tableName) << "\" (\n" ;

	bool firstField = true ;

	// Auto-generated id
	if(mapping->surrogateIdFieldName!=boost::none)
	{
		sql << "  \"" << mapping->surrogateIdFieldName.get() << "\" " << Types::autoincrementType() << " primary key " ;
		firstField = false ;
	}

	std::string primaryKey ;
	for(auto& field : mapping->fields)
	{
		if(!firstField)
			sql << ",\n" ;

//		std::cout << field.debug() << std::endl ;

		std::string sqlType=field.sqlType() ;
		if(field.isForeignKey() && !(field.fkConstraints() & mapping::FKNotNull))
		{
			if(sqlType.length()>9 && sqlType.substr(sqlType.length()-9)==" not null")
				sqlType = sqlType.substr(0, sqlType.length()-9) ;
		}

		sql << "  \"" << field.name() << "\" " << sqlType ;

		firstField = false ;

		if(field.isNaturalIdField())
		{
			if(!primaryKey.empty())
				primaryKey += ", " ;
			primaryKey += "\""+field.name()+"\"" ;
		}
	}

	if(!primaryKey.empty())
	{
		if(!firstField)
			sql << ",\n" ;

		sql << "  primary key (" << primaryKey << ")" ;
	}

	int i=0 ;
	for(auto& field : mapping->fields)
	{
		if(field.isForeignKey() && createConstraints)
		{
			if(!firstField)
				sql << ",\n" ;

			unsigned firstI=i ;
			i = findLastForeignKeyField(mapping, field, firstI) ;
			sql << "  " << constraintString(mapping, field, firstI, i) ;

			createTable(mapping, tablesCreated, sout, false);
		}
		else
			++i ;
	}

	sql << "\n)" ;

	executeSql(sql, sout) ;
}

//constraint fk_... foreign key ( ..., .. , .. ) references (..)
std::string connection::constraintString(MappingInfoPtr mapping, const mapping::FieldInfo& field, unsigned fromIndex, unsigned toIndex)
{
	std::stringstream sql;

	sql << "constraint \"fk_" << mapping->tableName << "_" << field.foreignKeyName() << "\"" << " foreign key (\"" << field.name() << "\"" ;

	for(unsigned i=fromIndex+1 ; i<toIndex ; ++i)
	{
		const mapping::FieldInfo& nextField=mapping->fields[i] ;
		sql << ", \"" << nextField.name() << "\"" ;
	}

	MappingInfoPtr otherMapping=getMapping(field.foreignKeyTable()) ;

	sql << ") references \"" << quoteSchemaDot(field.foreignKeyTable()) << "\" (" << otherMapping->primaryKeys() << ")" ;

	if(field.fkConstraints() & mapping::FKOnUpdateCascade)
		sql << " on update cascade" ;
	else if(field.fkConstraints() & mapping::FKOnUpdateSetNull)
		sql << " on update set null" ;

	if(field.fkConstraints() & mapping::FKOnDeleteCascade)
		sql << " on delete cascade" ;
	else if(field.fkConstraints() & mapping::FKOnDeleteSetNull)
		sql << " on delete set null" ;

	sql << " deferrable initially deferred" ;

	return sql.str();
}

unsigned connection::findLastForeignKeyField(MappingInfoPtr mapping, const mapping::FieldInfo& field, unsigned index)
{
	while(index<mapping->fields.size())
	{
		const mapping::FieldInfo& nextField = mapping->fields[index];
		if(nextField.foreignKeyName()==field.foreignKeyName())
		{
			++index;
		}
		else
			break;
	}

	return index;
}

void connection::createTables()
{
	initSchema() ;

	std::set<std::string> tablesCreated ;

	for(auto& registry : classRegistry_)
		createTable(registry.second, tablesCreated, 0, false) ;

	for(auto& registry : classRegistry_)
		createRelations(registry.second, tablesCreated, 0) ;
}

void connection::resolveJoinIds(MappingInfoPtr mapping)
{
	for(auto& set : mapping->sets)
	{
		if(set.type==mapping::ManyToMany)
		{
			MappingInfoPtr other=getMapping(set.tableName) ;

			for(auto& otherSet : mapping->sets)
			{
				if(otherSet.joinName==set.joinName)
				{
					// second check make sure we find the other id if Many-To-Many between
					// same table
					if(mapping!=other || &set!=&otherSet)
					{
						set.joinOtherId = otherSet.joinSelfId ;
						set.otherFkConstraints = otherSet.fkConstraints ;
						break ;
					}
				}
			}
		}
	}
}

void connection::createRelations(MappingInfoPtr mapping, std::set<std::string>& tablesCreated, std::ostream *sout)
{
	for(auto& set : mapping->sets)
	{
		if(set.type==mapping::ManyToMany)
		{
			if(tablesCreated.count(set.joinName)==0)
			{
				MappingInfoPtr other=getMapping(set.tableName) ;

				createJoinTable(set.joinName, mapping, other, set.joinSelfId, set.joinOtherId, set.fkConstraints, set.otherFkConstraints, tablesCreated, sout) ;
			}
		}
	}

	unsigned i=0 ;
	for(unsigned i=0 ; i<mapping->fields.size() ; )
	{
		const mapping::FieldInfo& field = mapping->fields[i] ;

		if(field.isForeignKey())
		{
			std::stringstream sql ;

			std::string table=quoteSchemaDot(mapping->tableName) ;

			sql << "alter table \"" << table << "\"" << " add " ;

			unsigned firstI=i ;
			i = findLastForeignKeyField(mapping, field, firstI) ;
			sql << constraintString(mapping, field, firstI, i) ;

			executeSql(sql, sout) ;
		}
		else
			++i ;
	}
}

void connection::createJoinTable(const std::string& joinName, MappingInfoPtr mapping1, MappingInfoPtr mapping2, const std::string& joinId1, const std::string& joinId2, int fkConstraints1, int fkConstraints2, std::set<std::string>& tablesCreated, std::ostream *sout)
{
	std::shared_ptr<mapping::MappingInfo> joinTableMapping=std::make_shared<mapping::MappingInfo>() ;

	joinTableMapping->tableName = joinName ;

	addJoinTableFields(joinTableMapping, mapping1, joinId1, "key1", fkConstraints1) ;
	addJoinTableFields(joinTableMapping, mapping2, joinId2, "key2", fkConstraints2) ;

	createTable(joinTableMapping, tablesCreated, sout, true) ;

	createJoinIndex(joinTableMapping, mapping1, joinId1, "key1", sout) ;
	createJoinIndex(joinTableMapping, mapping2, joinId2, "key2", sout) ;
}

void connection::createJoinIndex(MappingInfoPtr joinTableMapping, MappingInfoPtr mapping, const std::string& joinId, const std::string& foreignKeyName, std::ostream *sout)
{
	std::stringstream sql ;

	sql << "create index \"" << joinTableMapping->tableName << "_" << mapping->tableName ;

	if(!joinId.empty())
		sql << "_" << joinId ;

	sql << "\" on \"" << quoteSchemaDot(joinTableMapping->tableName) << "\" (" ;

	bool firstField=true ;
	for(auto& field : joinTableMapping->fields)
	{
		if(field.foreignKeyName()==foreignKeyName)
		{
			if(!firstField)
				sql << ", " ;
			firstField = false ;

			sql << "\"" << field.name() << "\"" ;
		}
	}

	sql << ")" ;

	executeSql(sql, sout) ;
}

std::vector<mapping::JoinId> connection::getJoinIds(MappingInfoPtr mapping, const std::string& joinId)
{
	std::vector<mapping::JoinId> result ;

	if(mapping->surrogateIdFieldName!=boost::none)
	{
		std::string idName ;

		if(joinId.empty())
			idName = std::string(mapping->tableName)+"_"+mapping->surrogateIdFieldName.get() ;
		else
			idName = joinId;

		result.push_back(mapping::JoinId(idName, mapping->surrogateIdFieldName.get(), traits::sql_value_traits<long long>::type(0))) ;
	}
	else
	{
		std::string foreignKeyName ;

		if(joinId.empty())
			foreignKeyName = std::string(mapping->tableName) ;
		else
			foreignKeyName = joinId ;

		for(auto& field : mapping->fields)
		{
			if(field.isNaturalIdField())
			{
				std::string idName=foreignKeyName+"_"+field.name() ;
				result.push_back(mapping::JoinId(idName, field.name(), field.sqlType())) ;
			}
		}
	}

	return result;
}

void connection::addJoinTableFields(MappingInfoPtr result, MappingInfoPtr mapping, const std::string& joinId, const std::string& keyName, int fkConstraints)
{
	std::vector<mapping::JoinId> joinIds=getJoinIds(mapping, joinId) ;

	for(unsigned i = 0 ; i<joinIds.size() ; ++i)
		result->fields.push_back(mapping::FieldInfo(joinIds[i].joinIdName, &typeid(long long), joinIds[i].sqlType, mapping->tableName, keyName, mapping::FieldInfo::NaturalId|mapping::FieldInfo::ForeignKey, fkConstraints)) ;
}

void connection::prepareInsertStatements(MappingInfoPtr mapping)
{
	std::stringstream sql ;

	std::string table=quoteSchemaDot(mapping->tableName) ;

	sql << "insert into \"" << table << "\" (" ;

	bool firstField=true ;
	for(auto& field : mapping->fields)
	{
		if(!firstField)
			sql << ", " ;
		sql << "\"" << field.name() << "\"" ;
		firstField = false ;
	}

	sql << ") values (" ;

	firstField = true ;
	for(auto& field : mapping->fields)
	{
		if(!firstField)
			sql << ", " ;
		sql << "?" ;
		firstField = false ;
	}

	sql << ")" ;

	if(mapping->surrogateIdFieldName!=boost::none)
		sql << Types::autoincrementInsertSuffix(mapping->surrogateIdFieldName.get()) ;

	mapping->statements.insert({mapping::MappingInfo::SqlInsert, stmt::Statement(*this, sql.str())}) ;
}

void connection::prepareUpdateStatements(MappingInfoPtr mapping)
{
	std::stringstream sql ;

	std::string table=quoteSchemaDot(mapping->tableName) ;

	sql << "update \"" << table << "\" set " ;

	bool firstField=true ;
	for(auto& field : mapping->fields)
	{
		if(!firstField)
			sql << ", " ;
		sql << "\"" << field.name() << "\" = ?" ;

		firstField = false ;
	}

	sql << " where " ;

	std::string idCondition ;

	if(mapping->surrogateIdFieldName==boost::none)
	{
		firstField = true ;
		for(auto& field : mapping->fields)
		{
			if(field.isNaturalIdField())
			{
				if(!firstField)
					idCondition += " and " ;
				idCondition += "\""+field.name()+"\" = ?" ;

				firstField = false ;
			}
		}

		if(firstField)
			throw Exception("Table "+std::string(mapping->tableName)+" is missing a natural id defined with dbo::id()") ;
	}
	else
		idCondition += std::string()+"\""+mapping->surrogateIdFieldName.get()+"\" = ?" ;

	mapping->idCondition = idCondition ;

	sql << idCondition ;

	mapping->statements.insert({mapping::MappingInfo::SqlUpdate, stmt::Statement(*this, sql.str())}) ;
}

void connection::prepareDeleteStatements(MappingInfoPtr mapping)
{
	std::stringstream sql ;

	std::string table=quoteSchemaDot(mapping->tableName) ;

	sql << "delete from \"" << table << "\" where " << mapping->idCondition ;

	mapping->statements.insert({mapping::MappingInfo::SqlDelete, stmt::Statement(*this, sql.str())}) ;
}

void connection::prepareSelectedByIdStatements(MappingInfoPtr mapping)
{
	std::stringstream sql ;

	std::string table=quoteSchemaDot(mapping->tableName) ;

	sql << "select ";

	bool firstField=true ;
	for(auto& field : mapping->fields)
	{
		if(!firstField)
			sql << ", " ;
		sql << "\"" << field.name() << "\"" ;
		firstField = false ;
	}

	sql << " from \"" << table << "\" where " << mapping->idCondition ;

	mapping->statements.insert({mapping::MappingInfo::SqlSelectById, stmt::Statement(*this, sql.str())}) ;
}

void connection::prepareCollectionsStatements(MappingInfoPtr mapping)
{

	for(auto& info : mapping->sets)
	{
		std::stringstream sql ;

		MappingInfoPtr otherMapping=getMapping(info.tableName) ;

		// select [surrogate id,] version, ... from other

		sql << "select " ;

		bool firstField = true ;
		if(otherMapping->surrogateIdFieldName!=boost::none)
		{
			sql << "\"" << otherMapping->surrogateIdFieldName << "\"" ;
			firstField = false ;
		}

		std::string fkConditions ;
		std::string other ;

		for(auto& field : otherMapping->fields)
		{
			if(!firstField)
				sql << ", " ;
			firstField = false ;

			sql << "\"" << field.name() << "\"" ;

			if(field.isForeignKey() && field.foreignKeyTable()==mapping->tableName)
			{
				if(field.foreignKeyName()==info.joinName)
				{
					if(!fkConditions.empty())
						fkConditions += " and " ;
					fkConditions += std::string("\"")+field.name()+"\" = ?" ;
				}
				else
				{
					if(!other.empty())
						other += " and " ;

					other += "'"+field.foreignKeyName()+"'" ;
				}
			}
		}

		sql << " from \"" << quoteSchemaDot(otherMapping->tableName) ;

		switch(info.type)
		{
		case mapping::ManyToOne:
			// where joinfield_id(s) = ?

			if(fkConditions.empty())
			{
				std::string msg = std::string()+"Relation mismatch for table '"+mapping->tableName+"': no matching belongsTo() found in table '"+otherMapping->tableName+"' with name '"+info.joinName+"'" ;

				if(!other.empty())
					msg += ", but did find with name "+other+"?" ;

				throw Exception(msg) ;
			}

			sql << "\" where " << fkConditions ;

			mapping->statements.insert({mapping::MappingInfo::FirstSqlSelectSet, stmt::Statement(*this, sql.str())}) ;
			break ;
		case mapping::ManyToMany:
			// (1) select for collection

			//     join "joinName" on "joinName"."joinId(s) = this."id(s)
			//     where joinfield_id(s) = ?

			std::string joinName=quoteSchemaDot(info.joinName) ;
			std::string tableName=quoteSchemaDot(info.tableName) ;

			sql << "\" join \"" << joinName << "\" on " ;

			std::vector<mapping::JoinId> otherJoinIds=getJoinIds(otherMapping, info.joinOtherId) ;

			if(otherJoinIds.size()>1)
				sql << "(" ;

			bool first=true ;
			for(auto& otherJoinId : otherJoinIds)
			{
				if(!first)
					sql << " and " ;
				sql << "\"" << joinName << "\".\"" << otherJoinId.joinIdName << "\" = \"" << tableName << "\".\"" << otherJoinId.tableIdName << "\"" ;
				first = false ;
			}

			if(otherJoinIds.size()>1)
				sql << ")" ;

			sql << " where " ;

			std::vector<mapping::JoinId> selfJoinIds = getJoinIds(mapping, info.joinSelfId) ;

			first = true ;
			for(auto& selfJoinId : selfJoinIds)
			{
				if(!first)
					sql << " and " ;
				sql << "\"" << joinName << "\".\"" << selfJoinId.joinIdName << "\" = ?" ;
				first = false ;
			}

			mapping->statements.insert({mapping::MappingInfo::FirstSqlSelectSet, stmt::Statement(*this, sql.str())}) ;

			// (2) insert into collection

			sql.str("") ;

			sql << "insert into \"" << joinName << "\" (" ;

			firstField = true ;
			for(unsigned i = 0 ; i<selfJoinIds.size() ; ++i)
			{
				if(!firstField)
					sql << ", " ;
				firstField = false ;

				sql << "\"" << selfJoinIds[i].joinIdName << "\"" ;
			}

			for(unsigned i = 0 ; i<otherJoinIds.size() ; ++i)
			{
				if(!firstField)
					sql << ", " ;
				firstField = false ;

				sql << "\"" << otherJoinIds[i].joinIdName << "\"" ;
			}

			sql << ") values (" ;

			for(unsigned i = 0 ; i<selfJoinIds.size()+otherJoinIds.size() ; ++i)
			{
				if(i!=0)
					sql << ", ";
				sql << "?" ;
			}

			sql << ")" ;

			mapping->statements.insert({mapping::MappingInfo::FirstSqlSelectSet, stmt::Statement(*this, sql.str())}) ;

			// (3) delete from collections

			sql.str("") ;

			sql << "delete from \"" << joinName << "\" where " ;

			firstField = true;
			for(unsigned i = 0 ; i<selfJoinIds.size() ; ++i)
			{
				if(!firstField)
					sql << " and ";
				firstField = false;

				sql << "\"" << selfJoinIds[i].joinIdName << "\" = ?" ;
			}

			for(unsigned i = 0 ; i<otherJoinIds.size() ; ++i)
			{
				if(!firstField)
					sql << " and " ;
				firstField = false;

				sql << "\"" << otherJoinIds[i].joinIdName << "\" = ?" ;
			}

			mapping->statements.insert({mapping::MappingInfo::FirstSqlSelectSet, stmt::Statement(*this, sql.str())}) ;
		}
	}

}

void connection::prepareStatements(MappingInfoPtr mapping)
{
	/*
	 * SqlInsert
	 */
	prepareInsertStatements(mapping) ;

	/*
	 * SqlUpdate
	 */
	prepareUpdateStatements(mapping) ;

	/*
	 * SqlDelete
	 */
	prepareDeleteStatements(mapping) ;

	/*
	 * SelectedById
	 */
	prepareSelectedByIdStatements(mapping) ;

	/*
	 * Collections SQL
	 */
	prepareCollectionsStatements(mapping) ;
}

void connection::initSchema()
{
	if(schemaInitialized_)
		return ;

	schemaInitialized_ = true ;

	for(auto& registry : classRegistry_)
		registry.second->init(*this) ;

	for(auto& registry : classRegistry_)
		resolveJoinIds(registry.second) ;

	for(auto& registry : classRegistry_)
		prepareStatements(registry.second) ;

}

std::string connection::tableCreationSql()
{
	initSchema() ;

	std::stringstream sout ;

	std::set<std::string> tablesCreated ;

	for(auto& registry : classRegistry_)
		createTable(registry.second, tablesCreated, &sout, false) ;

	for(auto& registry : classRegistry_)
		createRelations(registry.second, tablesCreated, &sout) ;

	return sout.str();
}


connection::MappingInfoPtr connection::getMapping(const std::string& tableName) const
{
	TableRegistry::const_iterator i = tableRegistry_.find(tableName);

	if(i!=tableRegistry_.end())
		return i->second;
	else
		return 0;
}

void connection::debug()
{
	std::stringstream ss ;
	std::string stab(1, ' ') ;
	std::string stab1(2, ' ') ;

	std::cout << "<connection>" << std::endl ;
	std::cout << stab << "classRegistry: " << std::endl ;
	for(auto& registry : classRegistry_)
	{
		std::cout << stab1 << "first: " << registry.first->name() << std::endl ;
		std::cout << stab1 << "second: " << std::endl ;
		std::cout << registry.second->debug(3) ;
	}

	// tableRegistry_ is a synonym of classRegistry_, not displayed
}

transaction& connection::transaction()
{
	if(conn_==nullptr)
		throw Exception("Database not connected") ;

	transaction_.open() ;

	return transaction_ ;
}

void connection::transaction(std::function<void()> func)
{
	transaction_.open() ;

	try {
		func() ;

		// auto commit at the end of the transaction
		transaction_.commit() ;
	} catch(const std::exception& e) {
		// should rollback to avoid letting the transaction in an unusable state
		transaction_.rollback() ;
		// throw without any parameters rethrow the original exception
		throw ;
	}
}
