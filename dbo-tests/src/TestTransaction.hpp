#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern std::string connection ;

// ----------------------------------------------------------------------------
class oSimpleTable
{
public:
	std::string value1 ;
	std::string value2 ;
	std::string value3 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value1, "value1") ;
		dbo::field(a, value2, "value2") ;
		dbo::field(a, value3, "value3") ;
	}
} ;
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestTransaction : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<oSimpleTable>("oSimpleTable") ;
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
	static dbo::connection db ;
} ;
dbo::connection TestTransaction::db ;


TEST_F(TestTransaction, TestTransactionEmpty) {

	db.transaction([&]() {
		// nothing
	}) ;
}


TEST_F(TestTransaction, TestTransactionOpen) {
	db.transaction([&]() {
		dbo::ptr<oSimpleTable> p=dbo::make_ptr<oSimpleTable>() ;
		db.insert(p) ;
	}) ;
}

