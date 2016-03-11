#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

extern std::string connection ;

// ----------------------------------------------------------------------------
class eSimpleTable
{
public:
	std::string name ;
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
	}
};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct eCompositeId
{
	std::string name ;
	dbo::ptr<eSimpleTable> simple_ptr ;

	bool operator==(const eCompositeId& other) const
	{
		return name==other.name && simple_ptr.id()==other.simple_ptr.id() ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, eCompositeId& key, const std::string& name, int size=-1)
{
	dbo::field(action, key.name, name + "_name") ;
	dbo::belongsTo(action, key.simple_ptr, name + "_simple", dbo::fk::OnDeleteCascade | dbo::fk::OnUpdateCascade | dbo::fk::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const eCompositeId& c)
{
	return o << "(" << c.name << ", " << c.simple_ptr << ")" ;
}

class eCompositeIdTable
{
public:
	eCompositeId composite_id ;

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
struct dbo_traits<eCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef eCompositeId IdType ;

	static IdType invalidId() { return eCompositeId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------

// The fixture for testing class Database.
class TestCompositeIdTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<eSimpleTable>("eSimpleTable") ;
		db.mapClass<eCompositeIdTable>("eCompositeIdTable") ;
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
dbo::connection TestCompositeIdTable::db ;


TEST_F(TestCompositeIdTable, TestSql) {
	dbo::connection db ;

	db.mapClass<eSimpleTable>("eSimpleTable") ;
	db.mapClass<eCompositeIdTable>("eCompositeIdTable") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestCompositeIdTable, TestInsertInvalidId) {
	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;

	// try to insert with an invalid id
	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestInsertDuplicate) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "duplicate" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<eCompositeIdTable> q=dbo::make_ptr<eCompositeIdTable>() ;
	q->composite_id.name = "duplicate" ;
	q->composite_id.simple_ptr = simple ;
	q->value = "value" ;
	ASSERT_THROW_V( db.insert(q), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestInsert) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "toto" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	ASSERT_FALSE( p.id()==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
}


TEST_F(TestCompositeIdTable, TestLoadInvalidId) {
	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	ASSERT_THROW_V( db.load<eCompositeIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestLoadNonExistingId) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.simple_ptr = simple ;
	ASSERT_THROW_V( db.load<eCompositeIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestLoad) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "load" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_FALSE( q.id()==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_TRUE( q.id()==p->composite_id ) ;
}


TEST_F(TestCompositeIdTable, TestUpdateInvalidId) {
	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;

	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestUpdateNonExistingId) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.simple_ptr = simple ;
	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestUpdate) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	simple->value = "TestUpdate" ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "TestUpdate" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "4" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_TRUE( q.loaded() ) ;
	ASSERT_TRUE( q->value=="4") ;
	ASSERT_TRUE( q.id()==p.id() ) ;
	ASSERT_FALSE( q.id()==q.invalidId ) ;
	ASSERT_TRUE(q.invalidId==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_FALSE( q->composite_id==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_FALSE( q->composite_id.simple_ptr.id()==dbo::traits::dbo_traits<eSimpleTable>::invalidId() ) ;

	dbo::ptr<eSimpleTable> newsimple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(newsimple) ) ;

	q.modify() ;
	q->value = "8" ;
	q->composite_id.simple_ptr = newsimple ;

	ASSERT_NO_THROW_V( db.update(q) ) ;

	dbo::ptr<eCompositeIdTable> r ;
	ASSERT_THROW_V( r = db.load<eCompositeIdTable>(p.id()), std::exception ) ;
	ASSERT_NO_THROW_V( r = db.load<eCompositeIdTable>(q.id()) ) ;
	ASSERT_TRUE( r->value=="8" ) ;
	ASSERT_FALSE( r.id().simple_ptr.id()==simple.id() ) ;
	ASSERT_TRUE( r.id().simple_ptr.id()==newsimple.id() ) ;
}

TEST_F(TestCompositeIdTable, TestRecursiveUpdate) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	simple->value = "TestRecursiveUpdate" ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "TestUpdate" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "4" ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;

	dbo::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_TRUE( q.loaded() ) ;
	ASSERT_TRUE( q->value=="4") ;
	ASSERT_TRUE( q.id()==p.id() ) ;
	ASSERT_FALSE( q.id()==q.invalidId ) ;
	ASSERT_TRUE( q.invalidId==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_FALSE( q->composite_id.simple_ptr.loaded() ) ;
	ASSERT_FALSE( q->composite_id==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_FALSE( q->composite_id.simple_ptr.id()==dbo::traits::dbo_traits<eSimpleTable>::invalidId() ) ;

	ASSERT_NO_THROW_V( db.load(q->composite_id.simple_ptr) ) ;
	ASSERT_TRUE( q->composite_id.simple_ptr.loaded() ) ;

	q.modify() ;
	q->value = "8" ;
	q->composite_id.simple_ptr.modify() ;
	q->composite_id.simple_ptr->value = "TestRecursiveUpdate new" ;
	ASSERT_NO_THROW_V( db.update(q, dbo::opt::Recursive) ) ;

	dbo::ptr<eCompositeIdTable> r ;
	ASSERT_NO_THROW_V( r = db.load<eCompositeIdTable>(q.id()) ) ;
	ASSERT_NO_THROW_V( db.load(q->composite_id.simple_ptr) ) ;
	ASSERT_TRUE( r->value=="8" ) ;
	ASSERT_FALSE( r.id().simple_ptr->value=="TestRecursiveUpdate" ) ;
	ASSERT_TRUE( r.id().simple_ptr->value=="TestRecursiveUpdate new" ) ;
}

TEST_F(TestCompositeIdTable, TestRemoveNull) {
	dbo::ptr<eCompositeIdTable> p ;

	ASSERT_THROW_V( db.remove<eCompositeIdTable>(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestRemoveInvalidId) {
	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;

	ASSERT_THROW_V( db.remove<eCompositeIdTable>(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestRemove) {
	dbo::ptr<eSimpleTable> simple=dbo::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo::ptr<eCompositeIdTable> p=dbo::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "remove" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "10" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;

	ASSERT_NO_THROW_V( db.remove<eCompositeIdTable>(q) ) ;
	ASSERT_TRUE( q.id()==dbo::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;

}
