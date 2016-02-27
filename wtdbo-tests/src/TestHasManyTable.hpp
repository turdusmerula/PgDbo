#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern wtdbo::backend::Postgres* db ;

class hComplexIdTable ;

// ----------------------------------------------------------------------------
class hSimpleTable
{
public:
	std::string name ;
	std::string value ;
	wtdbo::collection<wtdbo::ptr<hComplexIdTable>> list1 ;
	wtdbo::collection<wtdbo::ptr<hComplexIdTable>> list2 ;
	wtdbo::collection<wtdbo::ptr<hComplexIdTable>> list3 ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, name, "name") ;
		wtdbo::field(a, value, "value") ;
		wtdbo::hasMany(a, list1, wtdbo::ManyToOne, "list1") ;
		wtdbo::hasMany(a, list2, wtdbo::ManyToMany, "hSimpleTable_list2") ;
		wtdbo::hasMany(a, list3, wtdbo::ManyToMany, "hSimpleTable_list3") ;
	}
};
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
struct hComplexId
{
	std::string name ;
	int age ;

	bool operator<(const hComplexId &other) const
	{
		return true ;
	}

	bool operator==(const hComplexId& other) const
	{
		return name==other.name && age==other.age ;
	}
} ;

// explains how to store a Id in database
template <class Action>
void field(Action& action, hComplexId& id, const std::string& name, int size=-1)
{
	wtdbo::field(action, id.name, name + "_name") ;
	wtdbo::field(action, id.age, name + "_age") ;
}

// wtdbo needs this internally
std::ostream& operator<< (std::ostream& o, const hComplexId& c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

class hComplexIdTable
{
public:
	hComplexId composite_id ;
	wtdbo::ptr<hSimpleTable> simple_owner ;

	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::id(a, composite_id) ;
		wtdbo::belongsTo(a, simple_owner, "list1") ;
		wtdbo::field(a, value, "value") ;
	}
} ;

namespace wtdbo {
template<>
struct dbo_traits<hComplexIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef hComplexId IdType ;

	static IdType invalidId() { return hComplexId() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};

}
// ----------------------------------------------------------------------------

// The fixture for testing class Database.
class TestHasManyTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		session.setConnection(*db) ;
		session.mapClass<hSimpleTable>("hSimpleTable") ;
		session.mapClass<hComplexIdTable>("hComplexIdTable") ;
		session.createTables() ;
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
	static wtdbo::Session session ;
} ;
wtdbo::Session TestHasManyTable::session ;


TEST_F(TestHasManyTable, TestSql) {
	wtdbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<hSimpleTable>("hSimpleTable") ;
	session.mapClass<hComplexIdTable>("hComplexIdTable") ;

	std::cout << session.tableCreationSql() << std::endl ;
}

TEST_F(TestHasManyTable, TestInsertSimple) {
    wtdbo::Transaction transaction(session);

    hSimpleTable* simple=new hSimpleTable ;
	simple->name = "toto" ;
	simple->value = "me" ;
	ASSERT_NO_THROW_V( session.add(simple) ) ;

	transaction.commit() ;
}
