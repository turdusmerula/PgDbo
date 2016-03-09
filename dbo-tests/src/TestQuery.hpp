#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern std::string connection ;

// ----------------------------------------------------------------------------
class gSimpleTable
{
public:
	std::string value1 ;
	int value2 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value1, "value1") ;
		dbo::field(a, value2, "value2") ;
	}
} ;
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestQuery : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<gSimpleTable>("gSimpleTable") ;
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
dbo::connection TestQuery::db ;



TEST_F(TestQuery, TestSql) {
	dbo::connection db ;

	db.mapClass<gSimpleTable>("simple") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestQuery, TestFind) {
	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<gSimpleTable> p=dbo::make_ptr<gSimpleTable>() ;
		p->value1 = "TestFind" ;
		p->value2 = i ;
		ASSERT_NO_THROW_V( db.insert(p) ) ;
	}

	dbo::query q(db) ;
	ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1='TestFind'").execute() ) ;

	int i=0 ;
	while(q.hasrow())
	{
		i++ ;
		dbo::ptr<gSimpleTable> r ;
		ASSERT_NO_THROW_V( q.read(r) ) ;
		ASSERT_TRUE( r->value1=="TestFind" ) ;
		q.nextRow() ;
	}
	ASSERT_TRUE( i==10 ) ;
}

TEST_F(TestQuery, TestFindBind) {
	dbo::ptr<gSimpleTable> p=dbo::make_ptr<gSimpleTable>() ;
	p->value1 = "TestFindBind" ;
	p->value2 = 10 ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::query q(db) ;
	ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1=? and value2=?").bind("TestFindBind").bind(10).execute() ) ;

	dbo::ptr<gSimpleTable> r ;
	ASSERT_NO_THROW_V( q.read(r) ) ;
	ASSERT_TRUE( r->value1=="TestFindBind" ) ;
	ASSERT_TRUE( r->value2==10 ) ;
}

TEST_F(TestQuery, TestQuery) {
	dbo::ptr<gSimpleTable> p=dbo::make_ptr<gSimpleTable>() ;
	p->value1 = "TestQuery" ;
	p->value2 = 10 ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::query q(db) ;
	ASSERT_NO_THROW_V( q = db.query("select *, 15 as count from \"gSimpleTable\" as t1 inner join \"gSimpleTable\" as t2 on t1.id=t2.id where t1.value1=? and t1.value2=? ").bind("TestQuery").bind(10).execute() ) ;

	dbo::ptr<gSimpleTable> r, s, t ;
	int count=0 ;
	ASSERT_NO_THROW_V( q.read(r).read(s).read(count) ) ;
	ASSERT_THROW_V( q.read(t), std::exception ) ;
	ASSERT_TRUE( count==15 ) ;
	ASSERT_TRUE( r->value1=="TestQuery" ) ;
	ASSERT_TRUE( r->value2==10 ) ;
	ASSERT_TRUE( s->value1=="TestQuery" ) ;
	ASSERT_TRUE( s->value2==10 ) ;
}
