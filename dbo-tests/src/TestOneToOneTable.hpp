#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo/dbo.hpp>

extern std::string connection ;

class lSimpleTable ;
class lSimpleTable2 ;

// ----------------------------------------------------------------------------
class lSimpleTable
{
public:
	std::string value ;

	dbo::ptr<lSimpleTable2> object ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
		dbo::hasOne(a, object, "object", dbo::fk::NotNull|dbo::fk::OnDeleteCascade) ;
	}
} ;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class lSimpleTable2
{
public:
	std::string value ;
	dbo::weak_ptr<lSimpleTable> parent ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, parent, "object") ;
	}
} ;
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestOneToOneTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<lSimpleTable>("lSimpleTable") ;
		db.mapClass<lSimpleTable2>("lSimpleTable2") ;
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
dbo::connection TestOneToOneTable::db ;

TEST_F(TestOneToOneTable, TestSql) {
	dbo::connection db ;

	db.mapClass<lSimpleTable>("lSimpleTable") ;
	db.mapClass<lSimpleTable2>("lSimpleTable2") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestOneToOneTable, TestInsert) {
	dbo::ptr<lSimpleTable2> c=dbo::make_ptr<lSimpleTable2>() ;
	c->value = "TestInsert" ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	c->parent = p ;
	p->value = "TestInsert" ;
	p->object = c ;

//	ASSERT_TRUE( c->parent.expired() ) ;
	ASSERT_FALSE( p->object==nullptr ) ;
	ASSERT_TRUE( p->object.id()==dbo::ptr<lSimpleTable>::invalidId ) ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	ASSERT_NO_THROW_V( db.insert(c) ) ;

	ASSERT_TRUE( p->object.id()==c.id() ) ;
}

TEST_F(TestOneToOneTable, TestInsertFkNotNull) {
	dbo::ptr<lSimpleTable2> c=dbo::make_ptr<lSimpleTable2>() ;
	c->value = "TestInsertFkNotNull" ;

	ASSERT_THROW_V( db.insert(c), std::exception ) ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->value = "TestInsertFkNotNull" ;

	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestOneToOneTable, TestRecursiveInsert) {
	dbo::ptr<lSimpleTable2> c=dbo::make_ptr<lSimpleTable2>() ;
	c->value = "TestInsert" ;
	// With recursive mode there's no need to set parent, relation is made automatically
	//c->parent = p ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->value = "TestInsert" ;
	p->object = c ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;
}

