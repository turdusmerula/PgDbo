#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

class gSimpleTable
{
public:
	std::string value1 ;
	std::string value2 ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, value1, "value1") ;
		wtdbo::field(a, value2, "value2") ;
	}
} ;

extern wtdbo::backend::Postgres* db ;

// The fixture for testing class Database.
class TestQuery : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		session.setConnection(*db) ;

		session.mapClass<gSimpleTable>("gSimpleTable") ;
		session.createTables() ;
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
	static wtdbo::Session session ;
} ;

wtdbo::Session TestQuery::session ;

TEST_F(TestQuery, TestQuerySelect) {
    wtdbo::Transaction transaction(session);

    for(int i=0 ; i<10 ; i++)
    {
		gSimpleTable* p=new gSimpleTable ;
		p->value1 = "a" ;
		ASSERT_NO_THROW( session.add(p) ) ;
    }
	session.flush() ;

	auto q=session.find<gSimpleTable>() ;//.where("value1=''") ;

	transaction.commit() ;
}
