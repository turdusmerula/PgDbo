#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern wtdbo::backend::Postgres* db ;
wtdbo::Session session ;

class fSimpleTable
{
public:
	std::string value1 ;
	std::string value2 ;
	std::string value3 ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, value1, "value1") ;
		wtdbo::field(a, value2, "value2") ;
		wtdbo::field(a, value3, "value3") ;
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
    wtdbo::Transaction transaction(session);

    wtdbo::ptr<fSimpleTable> simple=new fSimpleTable ;

    simple.modify()->value1 = "a" ;
    simple.modify()->value2 = "b" ;
    simple.modify()->value3 = "c" ;

    std::cerr << simple->value1 << " - " << simple->value2 << " - " << simple->value3 << std::endl ;

    session.add(simple) ;

    transaction.commit() ;
}

TEST_F(TestRequest, TestLoad) {
	wtdbo::ptr<fSimpleTable> simple ;
	{
		wtdbo::Transaction transaction(session);
		simple = new fSimpleTable ;
		simple.modify()->value1 = "aa" ;
		simple.modify()->value2 = "bb" ;
		simple.modify()->value3 = "cc" ;

		session.add(simple) ;
		transaction.commit() ;
	}
	{
		wtdbo::Transaction transaction(session);
		wtdbo::ptr<fSimpleTable> read=session.load<fSimpleTable>(simple.id()) ;
		ASSERT_TRUE( read->value1=="aa" ) ;
		ASSERT_TRUE( read->value2=="bb" ) ;
		ASSERT_TRUE( read->value3=="cc" ) ;
		transaction.commit() ;
	}
	{
		wtdbo::Transaction transaction(session);
		wtdbo::ptr<fSimpleTable> read=session.load<fSimpleTable>(1000) ;
		ASSERT_TRUE( read->value1=="aa" ) ;
		ASSERT_TRUE( read->value2=="bb" ) ;
		ASSERT_TRUE( read->value3=="cc" ) ;
		transaction.commit() ;
	}

}

TEST_F(TestRequest, TestUpdate) {
	int id=-1 ;

	{
		wtdbo::Transaction transaction(session);

		wtdbo::ptr<fSimpleTable> simple=new fSimpleTable ;

		simple.modify()->value1 = "a" ;
		simple.modify()->value2 = "b" ;
		simple.modify()->value3 = "c" ;

		session.add(simple) ;

		transaction.commit();

		id = simple.id() ;
	}

	{
		wtdbo::Transaction transaction(session);

		wtdbo::ptr<fSimpleTable> simple=session.find<fSimpleTable>().where("id=?").bind(id) ;

		simple.modify()->value1 = "1" ;
		simple.modify()->value2 = "2" ;
		simple.modify()->value3 = "3" ;

	    std::cerr << simple->value1 << " - " << simple->value2 << " - " << simple->value3 << std::endl ;

		transaction.commit();
	}
}
