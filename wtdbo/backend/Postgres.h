/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 *
 * Contributed by: Hilary Cheng
 */
#ifndef DBO_BACKEND_POSTGRES_H_
#define DBO_BACKEND_POSTGRES_H_

#include <wtdbo/SqlConnection.h>
#include <wtdbo/SqlStatement.h>

struct pg_conn;
typedef struct pg_conn PGconn;

namespace wtdbo
{
namespace backend
{

/*! \class Postgres Dbo/backend/Postgres Dbo/backend/Postgres
 *  \brief A PostgreSQL connection
 *
 * This class provides the backend implementation for PostgreSQL databases.
 *
 * When applicable, exceptions from the backend will return the
 * five-character SQLSTATE error codes, as in
 * http://www.postgresql.org/docs/8.1/static/errcodes-appendix.html, in
 * Exception::code().
 *
 * \ingroup wtdbo
 */
class Postgres : public SqlConnection
{
public:
	/*! \brief Creates new PostgreSQL backend connection.
	 *
	 * The connection is not yet open, and requires a connect() before it
	 * can be used.
	 */
	Postgres();

	/*! \brief Opens a new PostgreSQL backend connection.
	 *
	 * The \p db may be any of the values supported by PQconnectdb().
	 */
	Postgres(const std::string& db);

	/*! \brief Copies a PostgreSQL connection.
	 *
	 * This creates a new connection with the same settings as another
	 * connection.
	 *
	 * \sa clone()
	 */
	Postgres(const Postgres& other);

	/*! \brief Destructor.
	 *
	 * Closes the connection.
	 */
	~Postgres();

	virtual Postgres *clone() const;

	/*! \brief Tries to connect.
	 *
	 * Throws an exception if there was a problem, otherwise true.
	 * An example connecion string could be:
	 * "host=127.0.0.1 user=test password=test port=5432 dbname=test"
	 */
	bool connect(const std::string& db);

	/*! \brief Returns the underlying connection.
	 */
	PGconn *connection()
	{
		return conn_;
	}

	virtual void executeSql(const std::string &sql);

	virtual void startTransaction();
	virtual void commitTransaction();
	virtual void rollbackTransaction();

	virtual SqlStatement *prepareStatement(const std::string& sql);

	/** @name Methods that return dialect information
	 */
	//@{
	virtual std::string autoincrementSql() const;
	virtual std::vector<std::string>
	autoincrementCreateSequenceSql(const std::string &table, const std::string &id) const;
	virtual std::vector<std::string>
	autoincrementDropSequenceSql(const std::string &table, const std::string &id) const;
	virtual std::string autoincrementType() const;
	virtual std::string autoincrementInsertSuffix(const std::string& id) const;
	virtual const char *dateTimeType(SqlDateTimeType type) const;
	virtual const char *blobType() const;
	virtual bool supportAlterTable() const;
	virtual bool supportDeferrableFKConstraint() const;
	virtual bool requireSubqueryAlias() const;
	//@}

private:
	std::string connInfo_;
	PGconn *conn_;
};

}
}

#endif // DBO_BACKEND_POSTGRES_H_
