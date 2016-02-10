#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

// The fixture for testing class Database.
class TestCustomIdTable : public ::testing::Test
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

class CustomIdTable
{
public:
	std::string natural_id ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::id(a, natural_id) ;
	}

} ;

namespace dbo2 {
namespace traits {
template<>
struct dbo_traits<CustomIdTable> : public dbo_default_traits
{
	// define custom id type
	typedef std::string IdType ;

	static IdType invalidId() { return std::string() ; }

	// deactivate default id
	static boost::optional<std::string> surrogateIdField() { return boost::none ; }
};
}}

TEST_F(TestCustomIdTable, TestSql) {
	dbo2::database db ;

	db.mapClass<CustomIdTable>("customid") ;

	std::cout << db.tableCreationSql() << std::endl ;
}
