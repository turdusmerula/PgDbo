//h
#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

class hComplexIdTable ;
class hSimpleTable ;
class hCompositeIdTable ;

// ----------------------------------------------------------------------------
struct hCompositeId
{
	std::string name ;
	dbo2::ptr<hSimpleTable> simple_ptr ;

	bool operator==(const hCompositeId& other) const
	{
		return name==other.name && simple_ptr.id()==other.simple_ptr.id() ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, hCompositeId& id, const std::string& name, int size=-1)
{
	dbo2::field(action, id.name, name + "_name") ;
	dbo2::belongsTo(action, id.simple_ptr, name + "_simple", dbo2::fk::OnDeleteCascade | dbo2::fk::OnUpdateCascade | dbo2::fk::NotNull) ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const hCompositeId& c)
{
	return o << "(" << c.name << ", " << c.simple_ptr << ")" ;
}

class hCompositeIdTable
{
public:
	hCompositeId composite_id ;

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
	dbo2::field(action, id.name, name + "_name") ;
	dbo2::field(action, id.age, name + "_age") ;
}

// dbo needs this internally
std::ostream& operator<< (std::ostream& o, const hComplexId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

class hComplexIdTable
{
public:
	hComplexId composite_id ;
	dbo2::ptr<hSimpleTable> simple_owner ;
	dbo2::collection<hCompositeIdTable> list_owner ;

	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, composite_id) ;
		dbo2::belongsTo(a, simple_owner, "list1") ;	// other side of the ManyToOne list1
		dbo2::hasMany(a, list_owner, dbo2::ManyToMany, "list_owner") ;
		dbo2::field(a, value, "value") ;
	}
} ;

namespace dbo2 {
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
class hSimpleTable
{
public:
	std::string name ;
	std::string value ;
	dbo2::collection<hComplexIdTable> list1 ;
	dbo2::collection<hComplexIdTable> list2 ;
	dbo2::collection<hComplexIdTable> list3 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, name, "name") ;
		dbo2::field(a, value, "value") ;
		dbo2::hasMany(a, list1, dbo2::ManyToOne, "list1") ;
		dbo2::hasMany(a, list2, dbo2::ManyToMany, "hSimpleTable_list2") ;
		dbo2::hasMany(a, list3, dbo2::ManyToMany, "hSimpleTable_list3") ;
	}
};
// ----------------------------------------------------------------------------


// The fixture for testing class Database.
class TestHasManyTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<hSimpleTable>("hSimpleTable") ;
		db.mapClass<hComplexIdTable>("hComplexIdTable") ;
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
	static dbo2::connection db ;
} ;
dbo2::connection TestHasManyTable::db ;


TEST_F(TestHasManyTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<hSimpleTable>("hSimpleTable") ;
	db.mapClass<hComplexIdTable>("hComplexIdTable") ;
	db.mapClass<hCompositeIdTable>("hCompositeIdTable") ;
	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}

TEST_F(TestHasManyTable, TestInsertSimple) {
	dbo2::ptr<hSimpleTable> simple=dbo2::make_ptr<hSimpleTable>() ;
	simple->name = "toto" ;
	simple->value = "me" ;
	ASSERT_NO_THROW_V( db.insert(simple) ) ;
}
