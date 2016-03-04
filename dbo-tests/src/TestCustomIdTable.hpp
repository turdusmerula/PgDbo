#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo/dbo.hpp>

extern std::string connection ;

// ----------------------------------------------------------------------------
class bNaturalIdTable
{
public:
	std::string natural_id ;
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, natural_id) ;
		dbo::field(a, value, "value") ;
	}

} ;

namespace dbo {
namespace traits {
template<>
struct dbo_traits<bNaturalIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef std::string IdType ;

	static IdType invalidId() { return std::string() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class bCustomIdTable
{
public:
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
	}

} ;

namespace dbo {
namespace traits {
template<>
struct dbo_traits<bCustomIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef std::string IdType ;

	static IdType invalidId()
	{
		return "" ;
	}

	static boost::optional<std::string> surrogateIdField() { return std::string("custom_id_name") ; }
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
		db.mapClass<bNaturalIdTable>("bNaturalIdTable") ;
		db.mapClass<bCustomIdTable>("bCustomIdTable") ;
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
dbo::connection TestCustomIdTable::db ;

TEST_F(TestCustomIdTable, TestSql) {
	dbo::connection db ;

	db.mapClass<bNaturalIdTable>("bNaturalIdTable") ;
	db.mapClass<bCustomIdTable>("bCustomIdTable") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestCustomIdTable, TestInsertInvalidId) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;

	// try to insert with an invalid id
	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestInsertDuplicate) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "duplicate" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<bNaturalIdTable> q=dbo::make_ptr<bNaturalIdTable>() ;
	q->natural_id = "duplicate" ;
	ASSERT_THROW_V( db.insert(q), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestInsert) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "toto" ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	ASSERT_TRUE( p.id()!=dbo::traits::dbo_traits<bNaturalIdTable>::invalidId() ) ;
	ASSERT_TRUE( p.id()==p->natural_id ) ;
}


TEST_F(TestCustomIdTable, TestLoadNaturalId) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "load natural" ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<bNaturalIdTable> q=db.load<bNaturalIdTable>(p.id()) ;
	ASSERT_TRUE( q.id()!=dbo::traits::dbo_traits<bNaturalIdTable>::invalidId() ) ;
	ASSERT_TRUE( q.id()==p->natural_id ) ;
}

TEST_F(TestCustomIdTable, TestLoadCustomId) {
	dbo::ptr<bCustomIdTable> p=dbo::make_ptr<bCustomIdTable>() ;
	p->value = "zuper gut" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<bCustomIdTable> q=db.load<bCustomIdTable>(p.id()) ;
	ASSERT_TRUE( q->value=="zuper gut" ) ;
	ASSERT_TRUE( q.id()!=dbo::traits::dbo_traits<bCustomIdTable>::invalidId() ) ;
}


TEST_F(TestCustomIdTable, TestUpdateInvalidId) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;

	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestUpdateNonExistingId) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "non exist" ;
	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestUpdate) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "update" ;
	p->value = "1" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<bNaturalIdTable> q=db.load<bNaturalIdTable>(p.id()) ;
	ASSERT_TRUE( q->value=="1") ;
	q->value = "2" ;

	ASSERT_NO_THROW_V( db.update(q) ) ;

	dbo::ptr<bNaturalIdTable> r=db.load<bNaturalIdTable>(p.id()) ;
	ASSERT_TRUE( r->value=="2") ;
}


TEST_F(TestCustomIdTable, TestRemoveNull) {
	dbo::ptr<bNaturalIdTable> p ;

	ASSERT_THROW_V( db.remove<bNaturalIdTable>(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestRemoveInvalidId) {
	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;

	ASSERT_THROW_V( db.remove<bNaturalIdTable>(p), std::exception ) ;
}

TEST_F(TestCustomIdTable, TestRemove) {

	dbo::ptr<bNaturalIdTable> p=dbo::make_ptr<bNaturalIdTable>() ;
	p->natural_id = "remove" ;
	p->value = "10" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<bNaturalIdTable> q=db.load<bNaturalIdTable>(p.id()) ;

	ASSERT_NO_THROW_V( db.remove<bNaturalIdTable>(q) ) ;
	ASSERT_TRUE( q.id()==dbo::traits::dbo_traits<bNaturalIdTable>::invalidId() ) ;

}
