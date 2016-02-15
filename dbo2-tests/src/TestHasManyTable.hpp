#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

// The fixture for testing class Database.
class TestHasManyTable : public ::testing::Test
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

class Session
{
public:
	std::string token ;

	dbo2::key<User> user ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, token, "token") ;
	    dbo2::belongsTo(a, user) ;
	}
} ;

class User
{
public:
	std::string name ;
//	dbo::collection<dbo::ptr<Session>> sessions ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, name, "name") ;
//	    dbo::hasMany(a, sessions, dbo::ManyToOne, "user") ;
	}
};


TEST_F(TestHasManyTable, TestSql) {
	dbo2::connection db ;

	db.mapClass<Session>("session") ;
	db.mapClass<User>("user") ;

	std::cout << db.tableCreationSql() << std::endl ;
}
