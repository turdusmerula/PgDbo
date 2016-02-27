#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern wtdbo::backend::Postgres* db ;

// The fixture for testing class Database.
class TestCustomIdTable : public ::testing::Test
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

// ----------------------------------------------------------------------------
class bCustomIdTable
{
public:
	std::string natural_id ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::id(a, natural_id) ;
	}

} ;

namespace wtdbo {
template<>
struct dbo_traits<bCustomIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef std::string IdType ;

	static IdType invalidId() { return std::string() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};
}
// ----------------------------------------------------------------------------

TEST_F(TestCustomIdTable, TestSql) {
	wtdbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<bCustomIdTable>("customid") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
