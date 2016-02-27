#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern wtdbo::backend::Postgres* db ;

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

// ----------------------------------------------------------------------------
struct cKey
{
	std::string name ;
	int age ;

	bool operator< (const cKey &other) const
	{
		return true ;
	}

	bool operator== (const cKey &other) const
	{
		return true ;
	}
} ;
std::ostream &operator<< (std::ostream &o, const cKey &c)
{
	return o << "(" << c.name << ", " << c.age << ")" ;
}

// explains how to store a Key in database
namespace wtdbo {
template <class Action>
void field(Action& action, cKey& key, const std::string& name, int size=-1)
{
	wtdbo::field(action, key.name, name + "_name") ;
	wtdbo::field(action, key.age, name + "_age") ;
}
}

class cCompositeIdTable
{
public:
	cKey composite_id ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::id(a, composite_id) ;
	}
}
;
namespace wtdbo {
template<>
struct dbo_traits<cCompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef cKey IdType ;

	static IdType invalidId() { return cKey() ; }

	// deactivate default id
	static const char* surrogateIdField() { return 0 ; }
};
}
// ----------------------------------------------------------------------------

TEST_F(TestCompositeIdTable, TestSql) {
	wtdbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<cCompositeIdTable>("compositeid") ;

	std::cout << session.tableCreationSql() << std::endl ;
}
