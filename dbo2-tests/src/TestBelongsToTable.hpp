#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

// The fixture for testing class Database.
class TestBelongsToTable : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
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
} ;

class User ;

// ----------------------------------------------------------------------------
struct dKey
{
	std::string name ;
	int age ;
} ;

// explains how to store a Key in database
template <class Action>
void field(Action& action, dKey& key, const std::string& name, int size=-1)
{
	dbo2::field(action, key.name, name + "_name") ;
	dbo2::field(action, key.age, name + "_age") ;
}

class dCompositeIdTable
{
public:
	dKey composite_id ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, composite_id) ;
	}
} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<dCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef dKey IdType ;

	static IdType invalidId() { return dKey() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}
// ----------------------------------------------------------------------------

class dUser
{
public:
	std::string name ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, name, "name") ;
	}
};
// ----------------------------------------------------------------------------


class dSession
{
public:
	std::string token ;

	dbo2::ptr<dUser> user ;
	dbo2::ptr<dCompositeIdTable> composite ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, token, "token") ;
	    dbo2::belongsTo(a, user) ;
	    dbo2::belongsTo(a, composite) ;
	}
} ;
// ----------------------------------------------------------------------------


TEST_F(TestBelongsToTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<dCompositeIdTable>("compositeid") ;
	db.mapClass<dUser>("user") ;
	db.mapClass<dSession>("session") ;

	std::cout << db.tableCreationSql() << std::endl ;
	db.debug() ;
}
