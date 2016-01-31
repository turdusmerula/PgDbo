// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2010 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_SQL_CONNECTION_POOL_H_
#define DBO_SQL_CONNECTION_POOL_H_

#include <vector>

  namespace dbo {

class SqlConnection;

/*! \class SqlConnectionPool Dbo/SqlConnectionPool Dbo/SqlConnectionPool
 *  \brief Abstract base class for a SQL connection pool.
 *
 * An sql connection pool manages a pool of connections. It is shared
 * between multiple sessions to allow these sessions to use a
 * connection while handling a transaction. Note that a session only
 * needs a connection while in-transaction, and thus you only need as
 * much connections as the number of concurrent transactions.
 *
 * \ingroup dbo
 */
class  SqlConnectionPool
{
public:
  /*! \brief Destructor.
   */
  virtual ~SqlConnectionPool();

  /*! \brief Uses a connection from the pool.
   *
   * This returns a connection from the pool that can be used. If the
   * pool has no more connection available, the pool may decide to
   * grow or block until a connection is returned.
   *
   * This method is called by a Session when a new transaction is
   * started.
   */
  virtual SqlConnection *getConnection() = 0;

  /*! \brief Returns a connection to the pool.
   *
   * This returns a connection to the pool. This method is called by a
   * Session after a transaction has been finished.
   */
  virtual void returnConnection(SqlConnection *) = 0;  
  
  /*! \brief Prepares all connections in the pool for dropping the tables.
   */
  virtual void prepareForDropTables() const = 0;
};

  }

#endif // DBO_SQL_CONNECTION_POOL_H_
