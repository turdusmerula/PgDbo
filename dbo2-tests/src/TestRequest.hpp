#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern dbo2::connection db ;

class fSimpleTable
{
public:
	std::string value1 ;
	std::string value2 ;
	std::string value3 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, value1, "value1") ;
		dbo2::field(a, value2, "value2") ;
		dbo2::field(a, value3, "value3") ;
	}
} ;

// The fixture for testing class Database.
class TestRequest : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.mapClass<fSimpleTable>("fsimple") ;
		std::cout << db.tableCreationSql() << std::endl ;
		db.createTables() ;
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


TEST_F(TestRequest, TestPtr) {
	dbo2::ptr<fSimpleTable> p ;
	dbo2::ptr<fSimpleTable> q ;

	ASSERT_FALSE(p) ;

	p = dbo2::make_ptr<fSimpleTable>() ;
	ASSERT_TRUE((bool)p) ;

	q = p ;
	ASSERT_TRUE((bool)q) ;

	db.debug() ;

	p->value1 = "toto" ;
	db.insert(p) ;
}
