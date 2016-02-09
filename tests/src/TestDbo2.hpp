#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

// The fixture for testing class Database.
class TestDbo2 : public ::testing::Test
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

class Simple {
public:
	std::string name ;
	int value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, name, "name") ;
		dbo2::field(a, value, "value") ;
	}
} ;


TEST_F(TestDbo2, Test1) {
	dbo2::database db ;

	db.mapClass<Simple>("simple") ;

	std::cout << db.tableCreationSql() << std::endl ;
}
