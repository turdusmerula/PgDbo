#include <gtest/gtest.h>

#include <iostream>

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
} ;

// explains how to store a Key in database
template <class Action>
void field(Action& action, Key& key, const std::string& name, int size=-1)
{
	dbo2::field(action, key.name, name + "_name") ;
	dbo2::field(action, key.age, name + "_age") ;
}

class CompositeIdTable
{
public:
	Key composite_id ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, composite_id) ;
	}
} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<CompositeIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef Key IdType ;

	static IdType invalidId() { return Key() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};

}}

TEST_F(TestCompositeIdTable, TestSql) {
	dbo2::database db ;

	db.mapClass<CompositeIdTable>("compositeid") ;

	std::cout << db.tableCreationSql() << std::endl ;
}
