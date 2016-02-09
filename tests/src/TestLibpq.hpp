#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include <pqxx/pqxx>

// The fixture for testing class Database.
class TestLibpq : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
	}

	static void TearDownTestCase()
	{

	}

	virtual void SetUp()
	{
		// Code here will be called immediately after the constructor (right
		// before each test).
	}

	virtual void TearDown()
	{
		// Code here will be called immediately after each test (right
		// before the destructor).
	}

	// Objects declared here can be used by all tests in the test case for Foo.
} ;


TEST_F(TestLibpq, Test1) {
    const char *conninfo;
    PGconn     *conn;
    PGresult   *res;
    int         nFields;
    int         i,
                j;

    /*
     * If the user supplies a parameter on the command line, use it as the
     * conninfo string; otherwise default to setting dbname=postgres and using
     * environment variables or defaults for all other connection parameters.
     */
    conninfo = "host=localhost port=5432 dbname=dbotest user=dbotest password=dbotest";

    /* Make a connection to the database */
    conn = PQconnectdb(conninfo);

    /* Check to see that the backend connection was successfully made */
    if (PQstatus(conn) != CONNECTION_OK)
    {
        fprintf(stderr, "Connection to database failed: %s",
                PQerrorMessage(conn));
        PQfinish(conn);
        return ;
    }

    /*
     * Our test case here involves using a cursor, for which we must be inside
     * a transaction block.  We could do the whole thing with a single
     * PQexec() of "select * from pg_database", but that's too trivial to make
     * a good example.
     */

    /* Start a transaction block */
    res = PQexec(conn, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "BEGIN command failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return ;
    }

    /*
     * Should PQclear PGresult whenever it is no longer needed to avoid memory
     * leaks
     */
    PQclear(res);

    /*
     * Fetch rows from pg_database, the system catalog of databases
     */
    res = PQexec(conn, "DECLARE myportal CURSOR FOR select * from pg_database");
    if (PQresultStatus(res) != PGRES_COMMAND_OK)
    {
        fprintf(stderr, "DECLARE CURSOR failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return ;
    }
    PQclear(res);

    res = PQexec(conn, "FETCH ALL in myportal");
    if (PQresultStatus(res) != PGRES_TUPLES_OK)
    {
        fprintf(stderr, "FETCH ALL failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        return ;
    }

    /* first, print out the attribute names */
    nFields = PQnfields(res);
    for (i = 0; i < nFields; i++)
        printf("%-15s", PQfname(res, i));
    printf("\n\n");

    /* next, print out the rows */
    for (i = 0; i < PQntuples(res); i++)
    {
        for (j = 0; j < nFields; j++)
            printf("%-15s", PQgetvalue(res, i, j));
        printf("\n");
    }

    PQclear(res);

    /* close the portal ... we don't bother to check for errors ... */
    res = PQexec(conn, "CLOSE myportal");
    PQclear(res);

    /* end the transaction */
    res = PQexec(conn, "END");
    PQclear(res);

    /* close the connection to the database and cleanup */
    PQfinish(conn);
}

//TEST_F(TestLibpq, Test2) {
//    const char *conninfo;
//    PGconn     *conn;
//    PGresult   *res;
//    PGnotify   *notify;
//    int         nnotifies;
//
//    /*
//     * If the user supplies a parameter on the command line, use it as the
//     * conninfo string; otherwise default to setting dbname=postgres and using
//     * environment variables or defaults for all other connection parameters.
//     */
//    conninfo = "host=localhost port=5432 dbname=dbotest user=dbotest password=dbotest";
//
//    /* Make a connection to the database */
//    conn = PQconnectdb(conninfo);
//
//    /* Check to see that the backend connection was successfully made */
//    if (PQstatus(conn) != CONNECTION_OK)
//    {
//        fprintf(stderr, "Connection to database failed: %s",
//                PQerrorMessage(conn));
//        PQfinish(conn);
//        return ;
//    }
//
//    /*
//     * Issue LISTEN command to enable notifications from the rule's NOTIFY.
//     */
//    res = PQexec(conn, "LISTEN TBL2");
//    if (PQresultStatus(res) != PGRES_COMMAND_OK)
//    {
//        fprintf(stderr, "LISTEN command failed: %s", PQerrorMessage(conn));
//        PQclear(res);
//        PQfinish(conn);
//        return ;
//    }
//
//    /*
//     * should PQclear PGresult whenever it is no longer needed to avoid memory
//     * leaks
//     */
//    PQclear(res);
//
//    /* Quit after four notifies are received. */
//    nnotifies = 0;
//    while (nnotifies < 4)
//    {
//        /*
//         * Sleep until something happens on the connection.  We use select(2)
//         * to wait for input, but you could also use poll() or similar
//         * facilities.
//         */
//        int         sock;
//        fd_set      input_mask;
//
//        sock = PQsocket(conn);
//
//        if (sock < 0)
//            break;              /* shouldn't happen */
//
//        FD_ZERO(&input_mask);
//        FD_SET(sock, &input_mask);
//
//        if (select(sock + 1, &input_mask, NULL, NULL, NULL) < 0)
//        {
//            fprintf(stderr, "select() failed: %s\n", strerror(errno));
//            PQfinish(conn);
//            return ;
//        }
//
//        /* Now check for input */
//        PQconsumeInput(conn);
//        while ((notify = PQnotifies(conn)) != NULL)
//        {
//            fprintf(stderr,
//                    "ASYNC NOTIFY of '%s' received from backend PID %d\n",
//                    notify->relname, notify->be_pid);
//            PQfreemem(notify);
//            nnotifies++;
//        }
//    }
//
//    fprintf(stderr, "Done.\n");
//
//    /* close the connection to the database and cleanup */
//    PQfinish(conn);
//}

TEST_F(TestLibpq, TestConnection) {
	ASSERT_THROW( { pqxx::connection conn("host=none port=5432 dbname=dbotest user=dbotest password=dbotest") ; }, std::exception) ;

	ASSERT_NO_THROW( pqxx::connection conn("host=localhost port=5432 dbname=dbotest user=dbotest password=dbotest") ;) ;
}

TEST_F(TestLibpq, TestTransaction) {
	pqxx::connection conn("host=localhost port=5432 dbname=dbotest user=dbotest password=dbotest") ;

	pqxx::transaction<> tr(conn, "transaction") ;
}

TEST_F(TestLibpq, TestPreparedStatement) {
	pqxx::connection conn("host=localhost port=5432 dbname=dbotest user=dbotest password=dbotest") ;

	conn.prepare("find", "Select * from user where mail=$1") ;
}
