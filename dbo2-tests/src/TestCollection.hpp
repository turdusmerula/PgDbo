#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern std::string connection ;

// ----------------------------------------------------------------------------
class jSimpleTable
{
public:
	std::string value1 ;
	long long  value2 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, value1, "value1") ;
		dbo2::field(a, value2, "value2") ;
	}
} ;
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestCollection : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<jSimpleTable>("jSimpleTable") ;
		db.createTables() ;
		db.showQueries(true) ;
		db.showBindings(true) ;
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
	static dbo2::connection db ;
} ;
dbo2::connection TestCollection::db ;


TEST_F(TestCollection, TestBulkInsert) {
	dbo2::collection<jSimpleTable> c ;

	for(int i=0 ; i<100 ; i++)
	{
		dbo2::ptr<jSimpleTable> p=dbo2::make_ptr<jSimpleTable>() ;
		p->value1 = "TestBulkInsert" ;
		p->value2 = i ;
		c.push_back(p) ;
	}

	ASSERT_NO_THROW_V( db.insert(c) ) ;
}
