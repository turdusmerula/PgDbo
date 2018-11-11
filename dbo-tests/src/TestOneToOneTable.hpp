#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern std::string connection ;

class lComplexIdTable ;
class lCompositeParentTable ;
class lCompositeIdTable ;

class lSimpleTable ;
class lSimpleTable2 ;



// ----------------------------------------------------------------------------
struct lComplexId
{
	std::string name ;
	int age ;

	bool operator==(const lComplexId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, lComplexId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::field(action, id.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const lComplexId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<lComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef lComplexId IdType ;

	static IdType invalidId() { return lComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
namespace dbo {
namespace traits {
template<>
struct dbo_traits<lCompositeParentTable> : public dbo_default_traits
{
	// define custom id type
	typedef lComplexId IdType ;

	static IdType invalidId() { return lComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
namespace dbo {
namespace traits {
template<>
struct dbo_traits<lSimpleTable> : public dbo_default_traits
{
	// define custom id type
	typedef lComplexId IdType ;

	static IdType invalidId() { return lComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct lCompositeId
{
	std::string name ;
	dbo::ptr<lCompositeParentTable> parent ;

	bool operator==(const lCompositeId& other) const
	{
		return name==other.name && parent.id()==other.parent.id() ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, lCompositeId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::belongsTo(action, id.parent, name + "_parent", dbo::fk::OnDeleteCascade | dbo::fk::OnUpdateCascade | dbo::fk::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const lCompositeId& c)
{
	return o << "(" << c.name << ", " << c.parent << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<lCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef lCompositeId IdType ;

	static IdType invalidId() { return lCompositeId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class lComplexIdTable
{
public:
	lComplexId complex_id ;

	std::string value ;

	dbo::weak_ptr<lSimpleTable> complex_parent ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, complex_id) ;
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, complex_parent, "complex") ;
	}
} ;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class lCompositeParentTable
{
public:
	lComplexId complex_id ;

	std::string value ;


	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, complex_id) ;
		dbo::field(a, value, "value") ;
	}
} ;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class lCompositeIdTable
{
public:
	lCompositeId composite_id ;

	std::string value ;

	dbo::weak_ptr<lSimpleTable> composite_parent ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, composite_parent, "composite") ;
	}
} ;

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class lSimpleTable
{
public:
	lComplexId complex_id ;

	std::string name ;
	std::string value ;

	dbo::ptr<lComplexIdTable> complex ;
	dbo::ptr<lCompositeIdTable> composite ;
	dbo::ptr<lSimpleTable2> simple ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, complex_id) ;
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
		dbo::hasOne(a, complex, "complex", dbo::fk::NotNull|dbo::fk::OnDeleteCascade) ;
		dbo::hasOne(a, composite, "composite", dbo::fk::NotNull|dbo::fk::OnDeleteCascade) ;
		dbo::hasOne(a, simple, "simple", dbo::fk::NotNull|dbo::fk::OnDeleteCascade) ;
	}
};
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class lSimpleTable2
{
public:
	std::string value ;
	dbo::weak_ptr<lSimpleTable> simple_parent ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, simple_parent, "simple", dbo::fk::NotNull) ;
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
		db.mapClass<lComplexIdTable>("lComplexIdTable") ;
		db.mapClass<lCompositeParentTable>("lCompositeParentTable") ;
		db.mapClass<lCompositeIdTable>("lCompositeIdTable") ;
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

	db.mapClass<lComplexIdTable>("lComplexIdTable") ;
	db.mapClass<lCompositeParentTable>("lCompositeParentTable") ;
	db.mapClass<lCompositeIdTable>("lCompositeIdTable") ;
	db.mapClass<lSimpleTable>("lSimpleTable") ;
	db.mapClass<lSimpleTable2>("lSimpleTable2") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}


TEST_F(TestOneToOneTable, TestInsert) {
	dbo::ptr<lSimpleTable2> simple=dbo::make_ptr<lSimpleTable2>() ;
	simple->value = "TestInsert" ;

	dbo::ptr<lComplexIdTable> complex=dbo::make_ptr<lComplexIdTable>() ;
	complex->value = "TestInsert" ;
	complex->complex_id.age = 2 ;
	complex->complex_id.name = "TestInsert" ;

	dbo::ptr<lCompositeIdTable> composite=dbo::make_ptr<lCompositeIdTable>() ;
	composite->value = "TestInsert" ;
	composite->composite_id.name = "TestInsert" ;
	dbo::ptr<lCompositeParentTable> composite_parent=dbo::make_ptr<lCompositeParentTable>() ;
	composite_parent->value = "TestInsert" ;
	composite_parent->complex_id.age = 10 ;
	composite_parent->complex_id.name = "TestInsert" ;
	composite->composite_id.parent = composite_parent ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->complex_id.age = 100 ;
	p->complex_id.name = "TestInsert" ;
	simple->simple_parent = p ;
	complex->complex_parent = p ;
	composite->composite_parent = p ;

	p->value = "TestInsert" ;
	p->simple = simple ;
	p->complex = complex ;
	p->composite = composite ;

	ASSERT_FALSE( p->simple==nullptr ) ;
	ASSERT_FALSE( p->complex==nullptr ) ;
	ASSERT_FALSE( p->composite==nullptr ) ;

	ASSERT_TRUE( p->simple.id()==dbo::ptr<lSimpleTable2>::invalidId ) ;
	ASSERT_TRUE( p->complex.id()==dbo::ptr<lComplexIdTable>::invalidId ) ;
	ASSERT_TRUE( p->composite.id()==dbo::ptr<lCompositeIdTable>::invalidId ) ;

	ASSERT_NO_THROW_V( db.insert(p) ) ;
	ASSERT_FALSE( p.id()==p.invalidId ) ;
	ASSERT_TRUE( simple->simple_parent.id()==p.id() ) ;
	ASSERT_TRUE( complex->complex_parent.id()==p.id() ) ;
	ASSERT_TRUE( composite->composite_parent.id()==p.id() ) ;

	ASSERT_NO_THROW_V( db.insert(complex) ) ;
	ASSERT_NO_THROW_V( db.insert(composite->composite_id.parent) ) ;
	ASSERT_NO_THROW_V( db.insert(composite) ) ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;

//	ASSERT_TRUE( p->object.id()==c.id() ) ;
}

TEST_F(TestOneToOneTable, TestInsertFkNotNull) {
	dbo::ptr<lSimpleTable2> c=dbo::make_ptr<lSimpleTable2>() ;
	c->value = "TestInsertFkNotNull" ;

	ASSERT_THROW_V( db.insert(c), std::exception ) ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->value = "TestInsertFkNotNull" ;

	ASSERT_THROW_V( db.insert(p), std::exception ) ;
}

TEST_F(TestOneToOneTable, TestInsertDoubleInsert) {
	dbo::ptr<lSimpleTable2> simple=dbo::make_ptr<lSimpleTable2>() ;
	simple->value = "TestRecursiveInsert" ;
	// With recursive mode there's no need to set parent, relation is made automatically
	//simple->parent = p ;

	dbo::ptr<lComplexIdTable> complex=dbo::make_ptr<lComplexIdTable>() ;
	complex->value = "TestInsertDoubleInsert" ;
	complex->complex_id.age = 2 ;
	complex->complex_id.name = "TestInsertDoubleInsert" ;

	dbo::ptr<lCompositeIdTable> composite=dbo::make_ptr<lCompositeIdTable>() ;
	composite->value = "TestInsertDoubleInsert" ;
	composite->composite_id.name = "TestInsertDoubleInsert" ;
	dbo::ptr<lCompositeParentTable> composite_parent=dbo::make_ptr<lCompositeParentTable>() ;
	composite_parent->value = "TestInsertDoubleInsert" ;
	composite_parent->complex_id.age = 10 ;
	composite_parent->complex_id.name = "TestInsertDoubleInsert" ;
	composite->composite_id.parent = composite_parent ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->complex_id.age = 100 ;
	p->complex_id.name = "TestInsertDoubleInsert" ;
	p->value = "TestInsertDoubleInsert" ;
	p->simple = simple ;
	p->complex = complex ;
	p->composite = composite ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;

	dbo::ptr<lComplexIdTable> newcomplex=dbo::make_ptr<lComplexIdTable>() ;
	newcomplex->complex_parent = p ;
	newcomplex->value = "TestInsertDoubleInsert" ;
	newcomplex->complex_id.age = 2 ;
	newcomplex->complex_id.name = "TestInsertDoubleInsert" ;
	// TODO: should fail, add a constraint to ensure unicity
	ASSERT_THROW_V( db.insert(newcomplex, dbo::opt::Recursive), std::exception ) ;
}

TEST_F(TestOneToOneTable, TestRecursiveInsert) {
	dbo::ptr<lSimpleTable2> simple=dbo::make_ptr<lSimpleTable2>() ;
	simple->value = "TestRecursiveInsert" ;
	// With recursive mode there's no need to set parent, relation is made automatically
	//simple->parent = p ;

	dbo::ptr<lComplexIdTable> complex=dbo::make_ptr<lComplexIdTable>() ;
	complex->value = "TestRecursiveInsert" ;
	complex->complex_id.age = 2 ;
	complex->complex_id.name = "TestRecursiveInsert" ;

	dbo::ptr<lCompositeIdTable> composite=dbo::make_ptr<lCompositeIdTable>() ;
	composite->value = "TestRecursiveInsert" ;
	composite->composite_id.name = "TestRecursiveInsert" ;
	dbo::ptr<lCompositeParentTable> composite_parent=dbo::make_ptr<lCompositeParentTable>() ;
	composite_parent->value = "TestRecursiveInsert" ;
	composite_parent->complex_id.age = 10 ;
	composite_parent->complex_id.name = "TestRecursiveInsert" ;
	composite->composite_id.parent = composite_parent ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->complex_id.age = 100 ;
	p->complex_id.name = "TestRecursiveInsert" ;
	p->value = "TestRecursiveInsert" ;
	p->simple = simple ;
	p->complex = complex ;
	p->composite = composite ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;
}

TEST_F(TestOneToOneTable, TestSelect) {
	dbo::ptr<lSimpleTable2> simple=dbo::make_ptr<lSimpleTable2>() ;
	simple->value = "TestSelect" ;

	dbo::ptr<lComplexIdTable> complex=dbo::make_ptr<lComplexIdTable>() ;
	complex->value = "TestSelect" ;
	complex->complex_id.age = 2 ;
	complex->complex_id.name = "TestSelect" ;

	dbo::ptr<lCompositeIdTable> composite=dbo::make_ptr<lCompositeIdTable>() ;
	composite->value = "TestSelect" ;
	composite->composite_id.name = "TestSelect" ;
	dbo::ptr<lCompositeParentTable> composite_parent=dbo::make_ptr<lCompositeParentTable>() ;
	composite_parent->value = "TestSelect" ;
	composite_parent->complex_id.age = 10 ;
	composite_parent->complex_id.name = "TestSelect" ;
	composite->composite_id.parent = composite_parent ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->complex_id.age = 100 ;
	p->complex_id.name = "TestSelect" ;
	p->value = "TestSelect" ;
	p->simple = simple ;
	p->complex = complex ;
	p->composite = composite ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;

	dbo::ptr<lSimpleTable> q ;
	ASSERT_NO_THROW_V( q = db.load<lSimpleTable>(p.id()) ) ;
	ASSERT_TRUE( q.loaded() ) ;

	ASSERT_FALSE( q->simple.id()==q->simple.invalidId ) ;
	ASSERT_FALSE( q->complex.id()==q->complex.invalidId ) ;
	ASSERT_FALSE( q->composite.id()==q->composite.invalidId ) ;

	dbo::ptr<lComplexIdTable> c ;
	ASSERT_FALSE( q->complex.loaded() ) ;
	ASSERT_NO_THROW_V( c = db.load<lComplexIdTable>(q->complex) ) ;
	ASSERT_TRUE( q->complex.loaded() ) ;
	ASSERT_TRUE( c==q->complex ) ;

	ASSERT_FALSE( q->simple.loaded() ) ;
	ASSERT_NO_THROW_V( db.load<lSimpleTable2>(q->simple) ) ;
	ASSERT_TRUE( q->simple.loaded() ) ;

	ASSERT_FALSE( q->composite.loaded() ) ;
	ASSERT_NO_THROW_V( db.load<lCompositeIdTable>(q->composite) ) ;
	ASSERT_TRUE( q->composite.loaded() ) ;

	ASSERT_TRUE( dbo::ptr<lSimpleTable2>::invalidId==dbo::traits::dbo_traits<lSimpleTable2>::invalidId() ) ;
	ASSERT_TRUE( dbo::ptr<lComplexIdTable>::invalidId==dbo::traits::dbo_traits<lComplexIdTable>::invalidId() ) ;
	ASSERT_TRUE( dbo::ptr<lSimpleTable>::invalidId==dbo::traits::dbo_traits<lSimpleTable>::invalidId() ) ;
	ASSERT_TRUE( dbo::ptr<lCompositeIdTable>::invalidId==dbo::traits::dbo_traits<lCompositeIdTable>::invalidId() ) ;
}

TEST_F(TestOneToOneTable, TestUpdateNull) {
	// TODO
	dbo::ptr<lSimpleTable2> simple=dbo::make_ptr<lSimpleTable2>() ;
	simple->value = "TestUpdate" ;

	dbo::ptr<lComplexIdTable> complex=dbo::make_ptr<lComplexIdTable>() ;
	complex->value = "TestUpdate" ;
	complex->complex_id.age = 2 ;
	complex->complex_id.name = "TestUpdate" ;

	dbo::ptr<lCompositeIdTable> composite=dbo::make_ptr<lCompositeIdTable>() ;
	composite->value = "TestUpdate" ;
	composite->composite_id.name = "TestUpdate" ;
	dbo::ptr<lCompositeParentTable> composite_parent=dbo::make_ptr<lCompositeParentTable>() ;
	composite_parent->value = "TestUpdate" ;
	composite_parent->complex_id.age = 10 ;
	composite_parent->complex_id.name = "TestUpdate" ;
	composite->composite_id.parent = composite_parent ;

	dbo::ptr<lSimpleTable> p=dbo::make_ptr<lSimpleTable>() ;
	p->complex_id.age = 100 ;
	p->complex_id.name = "TestUpdate" ;
	p->value = "TestUpdate" ;
	p->simple = simple ;
	p->complex = complex ;
	p->composite = composite ;

	ASSERT_NO_THROW_V( db.insert(p, dbo::opt::Recursive) ) ;

	dbo::ptr<lSimpleTable> q ;
	ASSERT_NO_THROW_V( q = db.load<lSimpleTable>(p.id()) ) ;
	ASSERT_TRUE( q.loaded() ) ;

	q.modify() ;
	q->complex.reset() ;
	ASSERT_THROW_V( db.update(q), std::exception ) ;
}

