#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern dbo::backend::Postgres* db ;

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

	dbo::ptr<User> user ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, token, "token") ;
	    dbo::belongsTo(a, user) ;
	}
} ;

class User
{
public:
	std::string name ;
	dbo::collection<dbo::ptr<Session>> sessions ;
	dbo::collection<dbo::ptr<CompositeIdTable>> compositeids ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
	    dbo::hasMany(a, sessions, dbo::ManyToOne, "user") ;
	    dbo::hasMany(a, compositeids, dbo::ManyToOne, "user") ;
	}
};

TEST_F(TestHasManyTable, TestSql) {
	dbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<Session>("session") ;
	session.mapClass<User>("user") ;
	session.mapClass<CompositeIdTable>("compositeid") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
