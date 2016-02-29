#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>
#include <chrono>

#include <stdio.h>
#include <stdlib.h>
#include <thread>

#include <dbo/dbo.hpp>

extern std::string connection ;

class jCollectionTable ;
class jSimpleTable2 ;

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

	// Always use ref with hasMany to avoid memory leaks
	dbo::ref<jCollectionTable> parent_value ;
//	dbo::ptr<jSimpleTable2> parent_value2 ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, string_value, "string_value", 100) ;
		dbo::field(a, longlong_value, "longlong_value") ;
		dbo::field(a, int_value, "int_value") ;
		dbo::field(a, long_value, "long_value") ;
		dbo::field(a, short_value, "short_value") ;
		dbo::field(a, bool_value, "bool_value") ;
		dbo::field(a, float_value, "float_value") ;
		dbo::field(a, double_value, "double_value") ;
		dbo::field(a, size_t_value, "size_t_value") ;
		dbo::field(a, date_value, "date_value") ;
		dbo::field(a, ptime_value, "ptime_value") ;
		dbo::field(a, time_duration_value, "time_duration_value") ;
		dbo::field(a, vector_value, "vector_value") ;
		dbo::field(a, optional_value, "optional_value") ;
		dbo::field(a, enum_value, "enum_value") ;

		dbo::belongsTo(a, parent_value, "coll") ;
//		dbo::belongsTo(a, parent_value2) ;
	}
} ;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class jSimpleTable2
{
public:
	std::string value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
	}
} ;
// ----------------------------------------------------------------------------

// ----------------------------------------------------------------------------
class jCollectionTable
{
public:
	std::string value ;
	dbo::collection<jSimpleTable> coll ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, value, "value") ;
		dbo::hasMany(a, coll, dbo::ManyToOne, "coll") ;
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
		db.mapClass<jSimpleTable2>("jSimpleTable2") ;
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
	static dbo::connection db ;
} ;
dbo::connection TestCollection::db ;


TEST_F(TestCollection, TestBulkInsert) {
	dbo::collection<jSimpleTable> c ;

	for(int i=0 ; i<100 ; i++)
	{
		dbo::ptr<jSimpleTable> p=dbo::make_ptr<jSimpleTable>() ;

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

TEST_F(TestCollection, DISABLED_TestRecursiveInsert) {
	dbo::ptr<jCollectionTable> c=dbo::make_ptr<jCollectionTable>() ;
	c->value = "TestRecursiveInsert" ;

	for(int i=0 ; i<10 ; i++)
	{
		dbo::ptr<jSimpleTable> p=dbo::make_ptr<jSimpleTable>() ;
//		p->int_value = i ;
//		p->string_value = "TestRecursiveInsert" ;
		p->parent_value = c ;
//
//		p->parent_value2 = dbo::make_ptr<jSimpleTable2>() ;
//		p->parent_value2->value = "TestRecursiveInsert" ;
//
		c->coll.push_back(p) ;
	}

	db.insert(c, dbo::opt::Recursive) ;
	std::cout << " ------- " << std::endl ;
}

TEST_F(TestCollection, TestThreadPtr) {
	dbo::ptr<jCollectionTable> c=dbo::make_ptr<jCollectionTable>() ;

	typedef std::chrono::duration<double, std::ratio<1>> DurationSeconds ;

	auto th_alloc=[&](){
		auto start = std::chrono::high_resolution_clock::now() ;

		while(DurationSeconds(std::chrono::high_resolution_clock::now()-start).count()<5)
		{
			c = dbo::make_ptr<jCollectionTable>() ;
		}
	} ;

	auto th_share=[&](){
		auto start = std::chrono::high_resolution_clock::now() ;

		while(DurationSeconds(std::chrono::high_resolution_clock::now()-start).count()<5)
		{
			{
				dbo::ptr<jCollectionTable> d=c ;
			}
		}
	} ;

	std::thread th1(th_alloc) ;

	std::vector<std::shared_ptr<std::thread>> ths ;
	for(int i=0 ; i<10 ; i++)
		ths.push_back(std::make_shared<std::thread>(th_share)) ;
	th1.join() ;
	for(auto& th : ths)
		th->join() ;
}
