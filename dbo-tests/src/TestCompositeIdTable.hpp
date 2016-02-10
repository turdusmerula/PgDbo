#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern dbo::backend::Postgres* db ;

// The fixture for testing class Database.
class TestCompositeIdTable : public ::testing::Test
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

struct Key
{
	std::string name ;
	int age ;

	bool operator< (const Key &other) const
	{
		return true ;
	}

	bool operator== (const Key &other) const
	{
		return true ;
	}
} ;
std::ostream &operator<< (std::ostream &o, const Key &c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

// explains how to store a Key in database
namespace dbo {
template <class Action>
void field(Action& action, Key& key, const std::string& name, int size=-1)
{
	dbo::field(action, key.name, name + "_name") ;
	dbo::field(action, key.age, name + "_age") ;
}
}

class CompositeIdTable
{
public:
	Key composite_id ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::id(a, composite_id) ;
	}
}
;
namespace dbo {
template<>
struct dbo_traits<CompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef Key IdType ;

	static IdType invalidId() { return Key() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};
}

TEST_F(TestCompositeIdTable, TestSql) {
	dbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<CompositeIdTable>("compositeid") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
