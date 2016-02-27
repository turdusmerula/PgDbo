#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <dbo2/dbo.hpp>

extern std::string connection ;

class jCollectionTable ;

// ----------------------------------------------------------------------------
class jSimpleTable
{
public:
	std::string string_value ;
	long long  longlong_value ;
	int int_value ;
	long long_value ;
	short short_value ;
	bool bool_value ;
	float float_value ;
	double double_value ;
	size_t size_t_value ;
	boost::gregorian::date date_value ;		//date only
	boost::posix_time::ptime ptime_value ;	// date time
	boost::posix_time::time_duration time_duration_value ;	// time only
	std::vector<unsigned char> vector_value ;

	boost::optional<int> optional_value ;

	enum TypeEnum {
		Enum0 = 0,
		Enum1 = 1,
		Enum2 = 2
	};
	TypeEnum enum_value ;

	dbo2::ptr<jCollectionTable> parent_value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, string_value, "string_value", 100) ;
		dbo2::field(a, longlong_value, "longlong_value") ;
		dbo2::field(a, int_value, "int_value") ;
		dbo2::field(a, long_value, "long_value") ;
		dbo2::field(a, short_value, "short_value") ;
		dbo2::field(a, bool_value, "bool_value") ;
		dbo2::field(a, float_value, "float_value") ;
		dbo2::field(a, double_value, "double_value") ;
		dbo2::field(a, size_t_value, "size_t_value") ;
		dbo2::field(a, date_value, "date_value") ;
		dbo2::field(a, ptime_value, "ptime_value") ;
		dbo2::field(a, time_duration_value, "time_duration_value") ;
		dbo2::field(a, vector_value, "vector_value") ;
		dbo2::field(a, optional_value, "optional_value") ;
		dbo2::field(a, enum_value, "enum_value") ;

		dbo2::belongsTo(a, parent_value, "coll") ;
	}
} ;
// ----------------------------------------------------------------------------


class jCollectionTable
{
public:
	std::string value ;
	dbo2::collection<jSimpleTable> coll ;

	template<class Action>
	void persist(Action& a)
	{
		dbo2::field(a, value, "value") ;
		dbo2::hasMany(a, coll, dbo2::ManyToOne, "coll") ;
	}
} ;

// The fixture for testing class Database.
class TestCollection : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<jSimpleTable>("jSimpleTable") ;
		db.mapClass<jCollectionTable>("jCollectionTable") ;
		db.createTables() ;
		db.showQueries(true) ;
		db.showBindings(true) ;
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
	static dbo2::connection db ;
} ;
dbo2::connection TestCollection::db ;


TEST_F(TestCollection, TestBulkInsert) {
	dbo2::collection<jSimpleTable> c ;

	for(int i=0 ; i<100 ; i++)
	{
		dbo2::ptr<jSimpleTable> p=dbo2::make_ptr<jSimpleTable>() ;

		p->string_value = "TestBulkInsert\b\f\n\r\t\v;\\073" ;
		p->longlong_value = i ;
		p->int_value = 20 ;
		p->long_value = 30 ;
		p->short_value = 40 ;
		p->bool_value = true ;
		p->float_value = 40.5 ;
		p->double_value = 50.6e15 ;
		p->size_t_value = 60 ;
		p->date_value = boost::gregorian::day_clock::local_day() ;
		p->ptime_value = boost::posix_time::second_clock::local_time() ;
		p->time_duration_value = boost::posix_time::second_clock::local_time().time_of_day() ;
		for(int j=0 ; j<=255 ; j++)
			p->vector_value.push_back(j) ;
		if(i%2==0)
			p->optional_value = i ;
		p->enum_value = jSimpleTable::Enum2 ;

		c.push_back(p) ;
	}

	ASSERT_NO_THROW_V( db.insert(c) ) ;
	ASSERT_TRUE( c.size()==0 ) ;
	ASSERT_TRUE( c.empty() ) ;
}

TEST_F(TestCollection, TestRecursiveInsert) {
	dbo2::ptr<jCollectionTable> c=dbo2::make_ptr<jCollectionTable>() ;
	c->value = "TestFind" ;


}
