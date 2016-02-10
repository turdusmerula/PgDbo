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

// ----------------------------------------------------------------------------
struct dKey
{
	std::string name ;
	int age ;

	bool operator< (const dKey &other) const
	{
		return true ;
	}

	bool operator== (const dKey &other) const
	{
		return true ;
	}
} ;
std::ostream &operator<< (std::ostream &o, const dKey &c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

// explains how to store a Key in database
namespace dbo {
template <class Action>
void field(Action& action, dKey& key, const std::string& name, int size=-1)
{
	dbo::field(action, key.name, name + "_name") ;
	dbo::field(action, key.age, name + "_age") ;
}
}

class dCompositeIdTable
{
public:
	dKey composite_id ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
	}
}
;
namespace dbo {
template<>
struct dbo_traits<dCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef dKey IdType ;

	static IdType invalidId() { return dKey() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};
}
// ----------------------------------------------------------------------------

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
//	dbo::collection<dbo::ptr<CompositeIdTable>> compositeids ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
	    dbo::hasMany(a, sessions, dbo::ManyToOne, "user") ;
//	    dbo::hasMany(a, compositeids, dbo::ManyToOne, "user") ;
	}
};

TEST_F(TestHasManyTable, TestSql) {
	dbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<Session>("session") ;
	session.mapClass<User>("user") ;
	session.mapClass<dCompositeIdTable>("compositeid") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
