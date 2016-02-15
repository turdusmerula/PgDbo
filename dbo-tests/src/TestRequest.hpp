#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern dbo::backend::Postgres* db ;
dbo::Session session ;

class fSimpleTable
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

// The fixture for testing class Database.
class TestRequest : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		session.setConnection(*db) ;

		session.mapClass<fSimpleTable>("fsimple") ;
		std::cout << session.tableCreationSql() << std::endl ;
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
} ;

TEST_F(TestRequest, TestInsert) {
    dbo::Transaction transaction(session);

    dbo::ptr<fSimpleTable> simple=new fSimpleTable ;

    simple.modify()->value1 = "a" ;
    simple.modify()->value2 = "b" ;
    simple.modify()->value3 = "c" ;

    std::cerr << simple->value1 << " - " << simple->value2 << " - " << simple->value3 << std::endl ;

    session.add(simple) ;

    transaction.commit() ;
}

TEST_F(TestRequest, TestUpdate) {
	int id=-1 ;

	{
		dbo::Transaction transaction(session);

		dbo::ptr<fSimpleTable> simple=new fSimpleTable ;

		simple.modify()->value1 = "a" ;
		simple.modify()->value2 = "b" ;
		simple.modify()->value3 = "c" ;

		session.add(simple) ;

		transaction.commit();

		id = simple.id() ;
	}

	{
		dbo::Transaction transaction(session);

		dbo::ptr<fSimpleTable> simple=session.find<fSimpleTable>().where("id=?").bind(id) ;

		simple.modify()->value1 = "1" ;
		simple.modify()->value2 = "2" ;
		simple.modify()->value3 = "3" ;

	    std::cerr << simple->value1 << " - " << simple->value2 << " - " << simple->value3 << std::endl ;

		transaction.commit();
	}
}
