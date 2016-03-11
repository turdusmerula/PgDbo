//h
#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

class hComplexIdTable ;
class hSimpleTable ;
class hSimpleTable2 ;
class hCompositeParentTable ;
class hCompositeIdTable ;

// ----------------------------------------------------------------------------
struct hComplexId
{
	std::string name ;
	int age ;

	bool operator==(const hComplexId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, hComplexId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::field(action, id.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const hComplexId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<hComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef hComplexId IdType ;

	static IdType invalidId() { return hComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
namespace dbo {
namespace traits {
template<>
struct dbo_traits<hCompositeParentTable> : public dbo_default_traits
{
	// define custom id type
	typedef hComplexId IdType ;

	static IdType invalidId() { return hComplexId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct hCompositeId
{
	std::string name ;
	dbo::ptr<hCompositeParentTable> parent ;

	bool operator==(const hCompositeId& other) const
	{
		return name==other.name && parent.id()==other.parent.id() ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, hCompositeId& id, const std::string& name, int size=-1)
{
	dbo::field(action, id.name, name + "_name") ;
	dbo::belongsTo(action, id.parent, name + "_parent", dbo::fk::OnDeleteCascade | dbo::fk::OnUpdateCascade | dbo::fk::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const hCompositeId& c)
{
	return o << "(" << c.name << ", " << c.parent << ")" ;
}

namespace dbo {
namespace traits {
template<>
struct dbo_traits<hCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef hCompositeId IdType ;

	static IdType invalidId() { return hCompositeId() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class hComplexIdTable
{
public:
	hComplexId composite_id ;

	std::string value ;

	dbo::weak_ptr<hSimpleTable> owner ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, owner, "hComplexIdTable_list") ;	// other side of the ManyToOne
	}
} ;

// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class hCompositeParentTable
{
public:
	hComplexId complex_id ;

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
class hCompositeIdTable
{
public:
	hCompositeId composite_id ;

	std::string value ;

	dbo::weak_ptr<hSimpleTable2> owner ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
		dbo::field(a, value, "value") ;
		dbo::belongsTo(a, owner, "hCompositeIdTable_list") ;	// other side of the ManyToOne
	}
} ;
// ----------------------------------------------------------------------------


// ----------------------------------------------------------------------------
class hSimpleTable
{
public:
	std::string name ;
	std::string value ;
	dbo::collection<hComplexIdTable> list_complex ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_complex, dbo::ManyToOne, "hComplexIdTable_list") ;
	}
};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class hSimpleTable2
{
public:
	std::string name ;
	std::string value ;
	dbo::collection<hCompositeIdTable> list_composite ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, list_composite, dbo::ManyToOne, "hCompositeIdTable_list") ;
	}
};
// ----------------------------------------------------------------------------

// The fixture for testing class Database.
class TestManyToOneTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<hSimpleTable>("hSimpleTable") ;
		db.mapClass<hSimpleTable2>("hSimpleTable2") ;
		db.mapClass<hComplexIdTable>("hComplexIdTable") ;
		db.mapClass<hCompositeParentTable>("hCompositeParentTable") ;
		db.mapClass<hCompositeIdTable>("hCompositeIdTable") ;
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
dbo::connection TestManyToOneTable::db ;


TEST_F(TestManyToOneTable, TestSql) {
	dbo::connection db ;

	db.mapClass<hSimpleTable>("hSimpleTable") ;
	db.mapClass<hSimpleTable2>("hSimpleTable2") ;
	db.mapClass<hComplexIdTable>("hComplexIdTable") ;
	db.mapClass<hCompositeParentTable>("hCompositeParentTable") ;
	db.mapClass<hCompositeIdTable>("hCompositeIdTable") ;
	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestManyToOneTable, TestInsertSimple) {
	dbo::ptr<hSimpleTable> simple=dbo::make_ptr<hSimpleTable>() ;
	simple->name = "toto" ;
	simple->value = "TestInsertSimple" ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;
}

TEST_F(TestManyToOneTable, TestInsertSimpleCollection) {
	dbo::ptr<hSimpleTable> simple=dbo::make_ptr<hSimpleTable>() ;
	simple->name = "toto" ;
	simple->value = "TestInsertSimpleCollection" ;

	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<hComplexIdTable> complex=dbo::make_ptr<hComplexIdTable>() ;
		complex->composite_id.age = i ;
		complex->composite_id.name = "TestInsertSimpleCollection" ;

		simple->list_complex.push_back(complex) ;
	}

	ASSERT_NO_THROW_V( db.insert(simple, dbo::opt::Recursive)) ;
}

TEST_F(TestManyToOneTable, TestInsertSimple2Collection) {
	dbo::ptr<hSimpleTable2> simple=dbo::make_ptr<hSimpleTable2>() ;
	simple->name = "toto" ;
	simple->value = "TestInsertSimple2Collection" ;

	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<hCompositeParentTable> parent=dbo::make_ptr<hCompositeParentTable>() ;
		parent->value = "TestInsertSimple2Collection" ;
		parent->complex_id.age = i ;
		parent->complex_id.name = "TestInsertSimple2Collection" ;

		dbo::ptr<hCompositeIdTable> composite=dbo::make_ptr<hCompositeIdTable>() ;
		composite->composite_id.parent = parent ;
		composite->composite_id.name = "TestInsertSimple2Collection" ;

		simple->list_composite.push_back(composite) ;
	}

	ASSERT_NO_THROW_V( db.insert(simple, dbo::opt::Recursive)) ;
}

TEST_F(TestManyToOneTable, TestUpdate) {
	ASSERT_FALSE( true ) ;
}

TEST_F(TestManyToOneTable, TestUpdateRecusrsive) {
	ASSERT_FALSE( true ) ;
}

TEST_F(TestManyToOneTable, TestDelete) {
	ASSERT_FALSE( true ) ;
}

TEST_F(TestManyToOneTable, TestDeleteRecursive) {
	ASSERT_FALSE( true ) ;
}

TEST_F(TestManyToOneTable, TestQuery) {
	ASSERT_FALSE( true ) ;
}

