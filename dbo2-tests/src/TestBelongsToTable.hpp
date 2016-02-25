#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern std::string connection ;

// ----------------------------------------------------------------------------
struct dId
{
	std::string name ;
	int age ;

	bool operator==(const dId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, dId& key, const std::string& name, int size=-1)
{
	dbo2::field(action, key.name, name + "_name") ;
	dbo2::field(action, key.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const dId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

class dComplexIdTable
{
public:
	dId complex_id ;
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, complex_id) ;
		dbo2::field(a, value, "value") ;
	}
} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<dComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef dId IdType ;

	static IdType invalidId() { return dId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------

class dSimpleTable
{
public:
	std::string name ;
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, name, "name") ;
		dbo2::field(a, value, "value") ;
	}
};
// ----------------------------------------------------------------------------


class dBelongsToTable
{
public:
	std::string value ;

	dbo2::ptr<dSimpleTable> owner_simple ;
	dbo2::ptr<dComplexIdTable> owner_complex ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, value, "value") ;
	    dbo2::belongsTo(a, owner_simple) ;
	    dbo2::belongsTo(a, owner_complex) ;
	}
} ;
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestBelongsToTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<dComplexIdTable>("dComplexIdTable") ;
		db.mapClass<dSimpleTable>("dSimpleTable") ;
		db.mapClass<dBelongsToTable>("dBelongsToSimpleTable1") ;
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
dbo2::connection TestBelongsToTable::db ;

class User ;


TEST_F(TestBelongsToTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<dComplexIdTable>("dComplexIdTable") ;
	db.mapClass<dSimpleTable>("dSimpleTable") ;
	db.mapClass<dBelongsToTable>("dBelongsToSimpleTable1") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestBelongsToTable, TestInsertNull) {

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	// p->owner_simple left null
	// p->owner_complex left null
	p->value = "ok" ;
	ASSERT_THROW_V( db.insert(p), std::exception ) ;

}

TEST_F(TestBelongsToTable, TestInsert) {
	dbo2::ptr<dSimpleTable> owner_simple=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "toto" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple) ) ;

	dbo2::ptr<dComplexIdTable> owner_complex=dbo2::make_ptr<dComplexIdTable>() ;
	owner_complex->complex_id.name = "toto" ;
	owner_complex->complex_id.age = 36 ;
	owner_complex->value = "10" ;
	ASSERT_NO_THROW_V( db.insert(owner_complex) ) ;

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	p->owner_simple = owner_simple ;
	p->owner_complex = owner_complex ;
	p->value = "ok" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

}


TEST_F(TestBelongsToTable, TestLoad) {
	dbo2::ptr<dSimpleTable> owner_simple=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "load" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple) ) ;

	dbo2::ptr<dComplexIdTable> owner_complex=dbo2::make_ptr<dComplexIdTable>() ;
	owner_complex->complex_id.name = "load" ;
	owner_complex->complex_id.age = 36 ;
	owner_complex->value = "10" ;
	ASSERT_NO_THROW_V( db.insert(owner_complex) ) ;

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	p->owner_simple = owner_simple ;
	p->owner_complex = owner_complex ;
	p->value = "ok" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<dBelongsToTable> q ;
	ASSERT_NO_THROW_V( q=db.load<dBelongsToTable>(p.id()) ) ;
	ASSERT_FALSE( q.id()==dbo2::traits::dbo_traits<dBelongsToTable>::invalidId() ) ;
	ASSERT_TRUE( q.id()==p.id() ) ;

	ASSERT_FALSE( q->owner_complex.id()==dbo2::traits::dbo_traits<dComplexIdTable>::invalidId() ) ;
	ASSERT_TRUE( q->owner_complex.id()==owner_complex.id() ) ;
	ASSERT_FALSE( q->owner_complex.loaded() ) ;
	ASSERT_FALSE( q->owner_complex.orphan() ) ;

	ASSERT_FALSE( q->owner_simple.id()==dbo2::traits::dbo_traits<dSimpleTable>::invalidId() ) ;
	ASSERT_TRUE( q->owner_simple.id()==owner_simple.id() ) ;
	ASSERT_FALSE( q->owner_simple.loaded() ) ;
	ASSERT_FALSE( q->owner_simple.orphan() ) ;
}


TEST_F(TestBelongsToTable, TestUpdateNull) {
	dbo2::ptr<dSimpleTable> owner_simple=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "update null" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple) ) ;

	dbo2::ptr<dComplexIdTable> owner_complex=dbo2::make_ptr<dComplexIdTable>() ;
	owner_complex->complex_id.name = "update null" ;
	owner_complex->complex_id.age = 36 ;
	owner_complex->value = "10" ;
	ASSERT_NO_THROW_V( db.insert(owner_complex) ) ;

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	p->owner_simple = owner_simple ;
	p->owner_complex = owner_complex ;
	p->value = "ok" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<dBelongsToTable> q ;
	ASSERT_NO_THROW_V( q=db.load<dBelongsToTable>(p.id()) ) ;

	q->owner_complex = dbo2::make_ptr<dComplexIdTable>() ;	// set null pointer
	ASSERT_THROW_V( db.update(q), std::exception ) ;
}

TEST_F(TestBelongsToTable, TestUpdate) {
	dbo2::ptr<dSimpleTable> owner_simple=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "update" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple) ) ;

	dbo2::ptr<dComplexIdTable> owner_complex=dbo2::make_ptr<dComplexIdTable>() ;
	owner_complex->complex_id.name = "update" ;
	owner_complex->complex_id.age = 36 ;
	owner_complex->value = "10" ;
	ASSERT_NO_THROW_V( db.insert(owner_complex) ) ;

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	p->owner_simple = owner_simple ;
	p->owner_complex = owner_complex ;
	p->value = "ok" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;


	dbo2::ptr<dBelongsToTable> q ;
	ASSERT_NO_THROW_V( q=db.load<dBelongsToTable>(p.id()) ) ;

	dbo2::ptr<dSimpleTable> owner_simple2=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "update new" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple2) ) ;

	q->value = "yeah" ;
	q->owner_simple = owner_simple2 ;
	ASSERT_NO_THROW_V( db.update(q) ) ;

	dbo2::ptr<dBelongsToTable> r ;
	ASSERT_NO_THROW_V( r=db.load<dBelongsToTable>(p.id()) ) ;
	ASSERT_TRUE( r->value=="yeah" ) ;
	ASSERT_TRUE( q->owner_simple.id()==owner_simple2.id() ) ;
}


TEST_F(TestBelongsToTable, TestDelete) {
	dbo2::ptr<dSimpleTable> owner_simple=dbo2::make_ptr<dSimpleTable>() ;
	owner_simple->name = "delete" ;
	owner_simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(owner_simple) ) ;

	dbo2::ptr<dComplexIdTable> owner_complex=dbo2::make_ptr<dComplexIdTable>() ;
	owner_complex->complex_id.name = "delete" ;
	owner_complex->complex_id.age = 36 ;
	owner_complex->value = "10" ;
	ASSERT_NO_THROW_V( db.insert(owner_complex) ) ;

	dbo2::ptr<dBelongsToTable> p=dbo2::make_ptr<dBelongsToTable>() ;
	p->owner_simple = owner_simple ;
	p->owner_complex = owner_complex ;
	p->value = "ok" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;


	dbo2::ptr<dBelongsToTable> q ;
	ASSERT_NO_THROW_V( q=db.load<dBelongsToTable>(p.id()) ) ;

	ASSERT_NO_THROW_V( db.remove(q) ) ;
	ASSERT_FALSE( q.loaded() ) ;
	ASSERT_TRUE( q.orphan() ) ;
}
