#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>
#include <postgresql/libpq-fe.h>
#include <pqxx/pqxx>

extern std::string connection ;

// The fixture for testing class Database.
class TestLibpqxx : public ::testing::Test
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

TEST_F(TestLibpqxx, TestConnection) {
	ASSERT_THROW( { pqxx::connection conn(connection) ; }, std::exception) ;

	ASSERT_NO_THROW( pqxx::connection conn(connection) ;) ;
}

TEST_F(TestLibpqxx, TestTransaction) {
	pqxx::connection conn(connection) ;

	pqxx::transaction<> tr(conn, "transaction") ;
}

TEST_F(TestLibpqxx, TestPreparedStatement) {
	pqxx::connection conn(connection) ;

	conn.prepare("find", "Select * from user where mail=$1") ;
	conn.quote()
}
