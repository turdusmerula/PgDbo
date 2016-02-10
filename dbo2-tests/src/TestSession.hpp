#include <gtest/gtest.h>

#include <iostream>

#include <dbo/Dbo.h>

extern std::unique_ptr<dbo::FixedSqlConnectionPool> pool ;


class Simple {
public:
	std::string name ;
	int value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
	}
} ;



// The fixture for testing class Database.
class TestSession : public ::testing::Test
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

TEST_F(TestSession, TestCreateDatabase) {
	auto session=dbo::Session::create() ;

	session->mapClass<Simple>("simple") ;

	session->createTables() ;
}
