#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern std::string connection ;

// ----------------------------------------------------------------------------
class bCustomIdTable
{
public:
	std::string natural_id ;
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, natural_id) ;
		dbo2::field(a, value, "value") ;
	}

} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<bCustomIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef std::string IdType ;

	static IdType invalidId() { return std::string() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// The fixture for testing class Database.
class TestCustomIdTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<bCustomIdTable>("bCustomIdTable") ;
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
	static dbo2::connection db ;
} ;
dbo2::connection TestCustomIdTable::db ;

TEST_F(TestCustomIdTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<bCustomIdTable>("customid") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestCustomIdTable, TestInsertInvalidId) {
	dbo2::ptr<bCustomIdTable> p=dbo2::make_ptr<bCustomIdTable>() ;

	// try to insert with an invalid id
	ASSERT_THROW( db.insert(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestInsert) {
	dbo2::ptr<bCustomIdTable> p=dbo2::make_ptr<bCustomIdTable>() ;
	p->natural_id = "toto" ;

	ASSERT_NO_THROW( db.insert(p) ) ;

	ASSERT_TRUE( p.id()!=dbo2::traits::dbo_traits<bCustomIdTable>::invalidId() ) ;
	ASSERT_TRUE( p.id()==p->natural_id ) ;

}
