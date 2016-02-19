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
		dbo2::field(a, name, "name") ;
		dbo2::field(a, value, "value") ;
	}
};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct eCompositeKey
{
	std::string name ;
	dbo2::ptr<eSimpleTable> simple_ptr ;

	bool operator==(const eCompositeKey& other) const
	{
		return name==other.name && simple_ptr.id()==other.simple_ptr.id() ;
	}
} ;

// explains how to store a Key in database
template <class Action>
void field(Action& action, eCompositeKey& key, const std::string& name, int size=-1)
{
	dbo2::field(action, key.name, name + "_name") ;
	dbo2::belongsTo(action, key.simple_ptr, name + "_simple", dbo2::OnDeleteCascade | dbo2::OnUpdateCascade | dbo2::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const eCompositeKey& c)
{
	return o << "(" << c.name << ", " << c.simple_ptr << ")" ;
}

class eCompositeIdTable
{
public:
	eCompositeKey composite_id ;

	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, composite_id) ;
		dbo2::field(a, value, "value") ;
	}
} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<eCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef eCompositeKey IdType ;

	static IdType invalidId() { return eCompositeKey() ; }

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
	static dbo2::connection db ;
} ;
dbo2::connection TestCompositeIdTable::db ;


TEST_F(TestCompositeIdTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<eSimpleTable>("eSimpleTable") ;
	db.mapClass<eCompositeIdTable>("eCompositeIdTable") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestCompositeIdTable, TestInsertInvalidId) {
	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;

	// try to insert with an invalid id
	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestInsertDuplicate) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "duplicate" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;
	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<eCompositeIdTable> q=dbo2::make_ptr<eCompositeIdTable>() ;
	q->composite_id.name = "duplicate" ;
	q->composite_id.simple_ptr = simple ;
	q->value = "value" ;
	ASSERT_THROW_V( db.insert(q), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestInsert) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "toto" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	ASSERT_FALSE( p.id()==dbo2::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
}


TEST_F(TestCompositeIdTable, TestLoadInvalidId) {
	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	ASSERT_THROW_V( db.load<eCompositeIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestLoadNonExistingId) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.simple_ptr = simple ;
	ASSERT_THROW_V( db.load<eCompositeIdTable>(p.id()), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestLoad) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "load" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "value" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_FALSE( q.id()==dbo2::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;
	ASSERT_TRUE( q.id()==p->composite_id ) ;
}


TEST_F(TestCompositeIdTable, TestUpdateInvalidId) {
	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;

	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestUpdateNonExistingId) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "non exist" ;
	p->composite_id.simple_ptr = simple ;
	ASSERT_THROW_V( db.update(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestUpdate) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "update" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "4" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_TRUE( q->value=="4") ;
	q->value = "8" ;

	ASSERT_NO_THROW_V( db.update(q) ) ;

	dbo2::ptr<eCompositeIdTable> r=db.load<eCompositeIdTable>(p.id()) ;
	ASSERT_TRUE( r->value=="8" ) ;
}


TEST_F(TestCompositeIdTable, TestRemoveNull) {
	dbo2::ptr<eCompositeIdTable> p ;

	ASSERT_THROW_V( db.remove<eCompositeIdTable>(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestRemoveInvalidId) {
	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;

	ASSERT_THROW_V( db.remove<eCompositeIdTable>(p), std::exception ) ;
}

TEST_F(TestCompositeIdTable, TestRemove) {
	dbo2::ptr<eSimpleTable> simple=dbo2::make_ptr<eSimpleTable>() ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

	dbo2::ptr<eCompositeIdTable> p=dbo2::make_ptr<eCompositeIdTable>() ;
	p->composite_id.name = "remove" ;
	p->composite_id.simple_ptr = simple ;
	p->value = "10" ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;

	dbo2::ptr<eCompositeIdTable> q=db.load<eCompositeIdTable>(p.id()) ;

	ASSERT_NO_THROW_V( db.remove<eCompositeIdTable>(q) ) ;
	ASSERT_TRUE( q.id()==dbo2::traits::dbo_traits<eCompositeIdTable>::invalidId() ) ;

}
