//h
#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo/dbo.hpp>

class mComplexIdTable ;
class mSimpleTable ;
class mSimpleTable2 ;
class mCompositeParentTable ;
class mCompositeIdTable ;

// ----------------------------------------------------------------------------
struct mComplexId
{
	std::string name ;
	int age ;

	bool operator==(const mComplexId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, mComplexId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::field(action, id.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const mComplexId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<mComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef mComplexId IdType ;

	static IdType invalidId() { return mComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
namespace dbo {
namespace traits {
template<>
struct dbo_traits<mCompositeParentTable> : public dbo_default_traits
{
	// define custom id type
	typedef mComplexId IdType ;

	static IdType invalidId() { return mComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct mCompositeId
{
	std::string name ;
	dbo::ptr<mCompositeParentTable> parent ;

	bool operator==(const mCompositeId& other) const
	{
		return name==other.name && parent.id()==other.parent.id() ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, mCompositeId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::belongsTo(action, id.parent, name + "_parent", dbo::fk::OnDeleteCascade | dbo::fk::OnUpdateCascade | dbo::fk::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const mCompositeId& c)
{
	return o << "(" << c.name << ", " << c.parent << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<mCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef mCompositeId IdType ;

	static IdType invalidId() { return mCompositeId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class mComplexIdTable
{
public:
	mComplexId composite_id ;

	std::string value ;

	dbo::collection<mSimpleTable> list_simple ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_simple, dbo::ManyToMany, "mSimpleTable_mComplexIdTable2") ;
	}
} ;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class mCompositeParentTable
{
public:
	mComplexId complex_id ;

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
class mCompositeIdTable
{
public:
	mCompositeId composite_id ;

	std::string value ;

	dbo::collection<mSimpleTable2> list_simple2 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_simple2, dbo::ManyToMany, "mSimpleTable2_mCompositeIdTable") ;
	}
} ;

// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class mSimpleTable
{
public:
	std::string name ;
	std::string value ;
	dbo::collection<mComplexIdTable> list_complex ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_complex, dbo::ManyToMany, "mSimpleTable_mComplexIdTable") ;
	}
};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class mSimpleTable2
{
public:
	std::string name ;
	std::string value ;
	dbo::collection<mCompositeIdTable> list_composite ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_composite, dbo::ManyToMany, "mSimpleTable2_mCompositeIdTable") ;
	}
};
// ----------------------------------------------------------------------------

// mSimpleTable -> mComplexIdTable -> mCompositeIdTable

// The fixture for testing class Database.
class TestManyToManyTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<mSimpleTable>("mSimpleTable") ;
		db.mapClass<mSimpleTable2>("mSimpleTable2") ;
		db.mapClass<mComplexIdTable>("mComplexIdTable") ;
		db.mapClass<mCompositeParentTable>("mCompositeParentTable") ;
		db.mapClass<mCompositeIdTable>("mCompositeIdTable") ;
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
dbo::connection TestManyToManyTable::db ;


TEST_F(TestManyToManyTable, TestSql) {
	dbo::connection db ;

	db.mapClass<mSimpleTable>("mSimpleTable") ;
	db.mapClass<mSimpleTable2>("mSimpleTable2") ;
	db.mapClass<mComplexIdTable>("mComplexIdTable") ;
	db.mapClass<mCompositeParentTable>("mCompositeParentTable") ;
	db.mapClass<mCompositeIdTable>("mCompositeIdTable") ;
	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestManyToManyTable, TestInsertSimple) {
	dbo::ptr<mSimpleTable> simple=dbo::make_ptr<mSimpleTable>() ;
	simple->name = "toto" ;
	simple->value = "TestInsertSimple" ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;
}

TEST_F(TestManyToManyTable, TestInsert1ManytoMany) {
	dbo::ptr<mSimpleTable> simple=dbo::make_ptr<mSimpleTable>() ;
	simple->name = "toto" ;
	simple->value = "TestInsert1ManytoMany" ;

	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<mComplexIdTable> complex=dbo::make_ptr<mComplexIdTable>() ;
		complex->composite_id.age = i ;
		complex->composite_id.name = "TestInsert1ManytoMany" ;

		simple->list_complex.push_back(complex) ;
	}

	ASSERT_NO_THROW_V( db.insert(simple, dbo::opt::Recursive)) ;
}

TEST_F(TestManyToManyTable, TestInsert2ManytoMany) {
	dbo::ptr<mSimpleTable2> simple=dbo::make_ptr<mSimpleTable2>() ;
	simple->name = "toto" ;
	simple->value = "TestInsert2ManytoMany" ;

	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<mCompositeParentTable> parent=dbo::make_ptr<mCompositeParentTable>() ;
		parent->value = "TestInsert2ManytoMany" ;
		parent->complex_id.age = i ;
		parent->complex_id.name = "TestInsert2ManytoMany" ;

		dbo::ptr<mCompositeIdTable> composite=dbo::make_ptr<mCompositeIdTable>() ;
		composite->composite_id.parent = parent ;
		composite->composite_id.name = "TestInsert2ManytoMany" ;

		simple->list_composite.push_back(composite) ;
	}

	ASSERT_NO_THROW_V( db.insert(simple, dbo::opt::Recursive)) ;
}
