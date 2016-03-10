#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

extern std::string connection ;

// ----------------------------------------------------------------------------
struct cId
{
	std::string name ;
	int age ;

	bool operator==(const cId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, cId& key, const std::string& name, int size=-1)
{
	dbo::field(action, key.name, name + "_name") ;
	dbo::field(action, key.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const cId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

class cComplexIdTable
{
public:
	cId composite_id ;

	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
	}
} ;

namespace dbo {
namespace traits {
template<>
struct dbo_traits<cComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef cId IdType ;

	static IdType invalidId() { return cId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------

// The fixture for testing class Database.
class TestComplexIdTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<cComplexIdTable>("cComplexIdTable") ;
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
dbo::connection TestComplexIdTable::db ;


TEST_F(TestComplexIdTable, TestSql) {
	dbo::connection db ;

	db.mapClass<cComplexIdTable>("cComplexIdTable") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestComplexIdTable, TestInsertInvalidId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;

	// try to insert with an invalid id
	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestInsertDuplicate) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "duplicate" ;
	p->composite_id.age = 25 ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<cComplexIdTable> q=dbo::make_ptr<cComplexIdTable>() ;
	q->composite_id.name = "duplicate" ;
	q->composite_id.age = 25 ;
	ASSERT_THROW_V( db.insert(q), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestInsert) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "toto" ;
	p->composite_id.age = 10 ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	ASSERT_FALSE( p.id()==dbo::traits::dbo_traits<cComplexIdTable>::invalidId() ) ;
}


TEST_F(TestComplexIdTable, TestLoadInvalidId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	ASSERT_THROW_V( db.load<cComplexIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestLoadNonExistingId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.age = 1500 ;
	ASSERT_THROW_V( db.load<cComplexIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestLoad) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "load" ;
	p->composite_id.age = 15 ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<cComplexIdTable> q=db.load<cComplexIdTable>(p.id()) ;
	ASSERT_FALSE( q.id()==dbo::traits::dbo_traits<cComplexIdTable>::invalidId() ) ;
	ASSERT_TRUE( q.id()==p->composite_id ) ;
}


TEST_F(TestComplexIdTable, TestUpdateInvalidId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;

	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestUpdateNonExistingId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.age = 200 ;
	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestUpdate) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "update" ;
	p->composite_id.age = 36 ;
	p->value = "4" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<cComplexIdTable> q=db.load<cComplexIdTable>(p.id()) ;
	ASSERT_TRUE( q->value=="4") ;

	q.modify() ;
	q->value = "8" ;

	ASSERT_NO_THROW_V( db.update(q) ) ;

	dbo::ptr<cComplexIdTable> r=db.load<cComplexIdTable>(p.id()) ;
	ASSERT_TRUE( r->value=="8") ;
}


TEST_F(TestComplexIdTable, TestRemoveNull) {
	dbo::ptr<cComplexIdTable> p ;

	ASSERT_THROW_V( db.remove<cComplexIdTable>(p), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestRemoveInvalidId) {
	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;

	ASSERT_THROW_V( db.remove<cComplexIdTable>(p), std::exception ) ;
}

TEST_F(TestComplexIdTable, TestRemove) {

	dbo::ptr<cComplexIdTable> p=dbo::make_ptr<cComplexIdTable>() ;
	p->composite_id.name = "remove" ;
	p->composite_id.age = 36 ;
	p->value = "10" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<cComplexIdTable> q=db.load<cComplexIdTable>(p.id()) ;

	ASSERT_NO_THROW_V( db.remove<cComplexIdTable>(q) ) ;
	ASSERT_TRUE( q.id()==dbo::traits::dbo_traits<cComplexIdTable>::invalidId() ) ;

}
