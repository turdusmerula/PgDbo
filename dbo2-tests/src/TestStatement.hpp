#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern std::string connection ;

// ----------------------------------------------------------------------------
class iSimpleTable
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
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestStatement : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<iSimpleTable>("iSimpleTable") ;
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
dbo2::connection TestStatement::db ;



TEST_F(TestStatement, TestSql) {
	dbo2::connection db ;

	db.mapClass<iSimpleTable>("iSimpleTable") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestStatement, TestNamedStatement) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	dbo2::stmt::PreparedStatement stmt(db, "TestNamedStatement", "select * from \"iSimpleTable\" where id=?") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;

	// prepare statement
	ASSERT_FALSE( stmt.prepared() ) ;
	ASSERT_NO_THROW_V( stmt.prepare() ) ;
	ASSERT_TRUE( stmt.prepared() ) ;

	// execute statement
	ASSERT_NO_THROW_V( stmt.execute() ) ;
	ASSERT_TRUE( stmt.nextRow() ) ;
	ASSERT_FALSE( stmt.nextRow() ) ;

	// statement already exists
	ASSERT_THROW_V( stmt.prepare(), std::exception ) ;

	// reuse statement
	stmt.reset() ;
	ASSERT_TRUE( stmt.prepared() ) ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;
	ASSERT_NO_THROW_V( stmt.execute() ) ;
}

TEST_F(TestStatement, TestNameHashStatement) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	dbo2::stmt::PreparedStatement stmt(db, "select * from \"iSimpleTable\" where id=?") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;
	ASSERT_NO_THROW_V( stmt.prepare() ) ;
	ASSERT_NO_THROW_V( stmt.execute() ) ;

	// statement already exist, ok
	ASSERT_NO_THROW_V( stmt.prepare() ) ;
}

TEST_F(TestStatement, TestAnonymousStatement) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	dbo2::stmt::PreparedStatement stmt(db, "", "select * from \"iSimpleTable\" where id=?") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;
	ASSERT_NO_THROW( stmt.prepare() ) ;
	ASSERT_NO_THROW( stmt.execute() ) ;

	// anonymous statement is overwritten
	ASSERT_NO_THROW_V( stmt.prepare() ) ;
}

TEST_F(TestStatement, TestWrongPrepare) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	dbo2::stmt::PreparedStatement stmt(db, "TestWrongPrepare", "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;

	// prepare statement
	ASSERT_FALSE( stmt.prepared() ) ;
	ASSERT_THROW_V( stmt.prepare(), std::exception ) ;
	ASSERT_FALSE( stmt.prepared() ) ;

}

TEST_F(TestStatement, TestDoublePrepareHashed) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	// hashed name statement
	dbo2::stmt::PreparedStatement stmt1(db, "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt1.bind(boost::lexical_cast<std::string>(p.id())) ;
	stmt1.bind(boost::lexical_cast<std::string>("")) ;

	// prepare statement
	ASSERT_NO_THROW( stmt1.prepare() ) ;
	ASSERT_TRUE( stmt1.prepared() ) ;

	// Prepare same statement
	dbo2::stmt::PreparedStatement stmt2(db, "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt2.bind(boost::lexical_cast<std::string>(p.id())) ;
	stmt2.bind(boost::lexical_cast<std::string>("")) ;

	// prepare statement
	ASSERT_NO_THROW( stmt2.prepare() ) ;
	ASSERT_TRUE( stmt2.prepared() ) ;
}

TEST_F(TestStatement, TestDoublePrepareNamed) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	// hashed name statement
	dbo2::stmt::PreparedStatement stmt1(db, "TestDoublePrepareNamed", "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt1.bind(boost::lexical_cast<std::string>(p.id())) ;
	stmt1.bind(boost::lexical_cast<std::string>("")) ;

	// prepare statement
	ASSERT_NO_THROW( stmt1.prepare() ) ;
	ASSERT_TRUE( stmt1.prepared() ) ;

	// Prepare same statement
	dbo2::stmt::PreparedStatement stmt2(db, "TestDoublePrepareNamed", "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt2.bind(boost::lexical_cast<std::string>(p.id())) ;
	stmt2.bind(boost::lexical_cast<std::string>("")) ;

	// prepare statement
	ASSERT_THROW_V( stmt2.prepare(), std::exception ) ;
}

TEST_F(TestStatement, TestMixedParameters) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	p->value1 = "v" ;
	p->value2 = "w" ;
	p->value3 = p->value1 ;
	db.insert(p) ;

	// use direct naming and dbo "?" in the same request, this is usefull when you need to bind a value more than once
	// this is an example to show it works but prefer to put auto numbered parameters at the end
	dbo2::stmt::PreparedStatement stmt(db, "select * from \"iSimpleTable\" where value1=$1 and id=? and value2=$2 and value3=$1") ;
	stmt.bind("v") ;
	stmt.bind("w") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;

	// prepare statement
	ASSERT_NO_THROW_V( stmt.prepare() ) ;

	// execute
	ASSERT_NO_THROW_V( stmt.execute() ) ;
	ASSERT_TRUE( stmt.nextRow() ) ;
	ASSERT_FALSE( stmt.nextRow() ) ;
}

TEST_F(TestStatement, TestWrongBind) {
	dbo2::ptr<iSimpleTable> p=dbo2::make_ptr<iSimpleTable>() ;
	db.insert(p) ;

	dbo2::stmt::PreparedStatement stmt(db, "select * from \"iSimpleTable\" where id=? and value1=?") ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;
	stmt.bind("") ;

	// prepare statement
	ASSERT_NO_THROW_V( stmt.prepare() ) ;

	stmt.reset() ;
	stmt.bind(boost::lexical_cast<std::string>(p.id())) ;

	ASSERT_THROW_V( stmt.execute(), std::exception ) ;
}
