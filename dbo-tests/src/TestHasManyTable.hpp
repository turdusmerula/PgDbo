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

// ----------------------------------------------------------------------------
class eUser ;
class eSession ;

struct eKey
{
	std::string name ;
	int age ;

	bool operator< (const eKey &other) const
	{
		return true ;
	}

	bool operator== (const eKey &other) const
	{
		return true ;
	}
} ;
std::ostream &operator<< (std::ostream &o, const eKey &c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

// explains how to store a Key in database
namespace dbo {
template <class Action>
void field(Action& action, eKey& key, const std::string& name, int size=-1)
{
	dbo::field(action, key.name, name + "_name") ;
	dbo::field(action, key.age, name + "_age") ;
}
}

class eCompositeIdTable
{
public:
	eKey composite_id ;
	dbo::collection<dbo::ptr<eUser>> users ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
	    dbo::hasMany(a, users, dbo::ManyToMany, "user_compositeid") ;
	}
}
;
namespace dbo {
template<>
struct dbo_traits<eCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef eKey IdType ;

	static IdType invalidId() { return eKey() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};
}
// ----------------------------------------------------------------------------

class eUser
{
public:
	std::string name ;
	dbo::collection<dbo::ptr<eSession>> sessions ;
	dbo::collection<dbo::ptr<eCompositeIdTable>> compositeids ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, name, "name") ;
	    dbo::hasMany(a, sessions, dbo::ManyToOne, "user") ;
	    dbo::hasMany(a, compositeids, dbo::ManyToMany, "user_compositeid") ;
	}
};
// ----------------------------------------------------------------------------

class eSession
{
public:
	std::string token ;

	dbo::ptr<eUser> user ;
	dbo::ptr<eCompositeIdTable> composite ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, token, "token") ;
	    dbo::belongsTo(a, user) ;
	    dbo::belongsTo(a, composite) ;
	}
} ;


TEST_F(TestHasManyTable, TestSql) {
	dbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<eCompositeIdTable>("compositeid") ;
	session.mapClass<eUser>("user") ;
	session.mapClass<eSession>("session") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
