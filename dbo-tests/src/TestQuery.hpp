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
class gSimpleTable2
{
public:
	std::string value1 ;
	std::string value2 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value1, "value1") ;
		dbo::field(a, value2, "value2") ;
	}
} ;


// The fixture for testing class Database.
class TestQuery : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<gSimpleTable>("gSimpleTable") ;
		db.mapClass<gSimpleTable2>("gSimpleTable2") ;
		db.createTables() ;
		db.showQueries(true) ;
		db.showBindings(true) ;
		db.showResults(true) ;
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

TEST_F(TestQuery, TestFindIterator) {
	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<gSimpleTable> p=dbo::make_ptr<gSimpleTable>() ;
		p->value1 = "TestFindIterator" ;
		p->value2 = i ;
		ASSERT_NO_THROW_V( db.insert(p) ) ;
	}

	dbo::query q(db) ;
	ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1='TestFindIterator'").execute() ) ;

	int i=0 ;
	for(auto& row : q)
	{
		i++ ;
		dbo::ptr<gSimpleTable> r ;
		ASSERT_NO_THROW_V( row.read(r) ) ;
		ASSERT_TRUE( r->value1=="TestFindIterator" ) ;
	}
	ASSERT_TRUE( i==10 ) ;
}

TEST_F(TestQuery, TestFindBind2) {
	dbo::ptr<gSimpleTable2> p=dbo::make_ptr<gSimpleTable2>() ;
	p->value1 = "TestFindBind2" ;
	p->value2 = "20" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable2>("value1='TestFindBind2' and value2='20'").execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
	}

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable2>("value1=?").bind("TestFindBind2").execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
	}
	
	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable2>("value2=?").bind(20).execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
		dbo::ptr<gSimpleTable2> r ;
		ASSERT_NO_THROW_V( q.read(r) ) ;
		ASSERT_TRUE( r->value1=="TestFindBind2" ) ;
		ASSERT_TRUE( r->value2=="20" ) ;
	}

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable2>("value1=? and value2=?").bind("TestFindBind2").bind("20").execute() ) ;
		//ASSERT_TRUE( q.numRow()>0 ) ;
	
		dbo::ptr<gSimpleTable2> r ;
		ASSERT_NO_THROW_V( q.read(r) ) ;
		ASSERT_TRUE( r->value1=="TestFindBind2" ) ;
		ASSERT_TRUE( r->value2=="20" ) ;
	}
}

TEST_F(TestQuery, TestFindBind) {
	dbo::ptr<gSimpleTable> p=dbo::make_ptr<gSimpleTable>() ;
	p->value1 = "TestFindBind" ;
	p->value2 = 10 ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1='TestFindBind' and value2='10'").execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
	}

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1=?").bind("TestFindBind").execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
	}
	
	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value2=?").bind("10").execute() ) ;
		ASSERT_TRUE( q.numRow()>0 ) ;
	}

	{
		dbo::query q(db) ;
		ASSERT_NO_THROW_V( q = db.find<gSimpleTable>("value1=? and value2=?").bind("TestFindBind").bind(10).execute() ) ;
		//ASSERT_TRUE( q.numRow()>0 ) ;
	
		dbo::ptr<gSimpleTable> r ;
		ASSERT_NO_THROW_V( q.read(r) ) ;
		ASSERT_TRUE( r->value1=="TestFindBind" ) ;
		ASSERT_TRUE( r->value2==10 ) ;
	}
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
