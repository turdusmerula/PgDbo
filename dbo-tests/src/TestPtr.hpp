#include <gtest/gtest.h>
#include <gtest_extend.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

#include <chrono>
#include <thread>

#include <dbo/dbo.hpp>

extern std::string connection ;

class kSimpleTable2 ;

// ----------------------------------------------------------------------------
class kSimpleTable
{
public:
	std::string string_value ;

	dbo::ptr<kSimpleTable2> object ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, string_value, "string_value", 100) ;
	}
} ;
// ----------------------------------------------------------------------------

class kSimpleTable2
{
public:
	std::string string_value ;

	template<class Action>
	void persist(Action& a)
	{
		dbo::field(a, string_value, "string_value", 100) ;
	}
} ;

// The fixture for testing class Database.
class TestPtr : public ::testing::Test
{
public:
	static void SetUpTestCase()
	{
		db.connect(connection) ;
		db.mapClass<kSimpleTable>("kSimpleTable") ;
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
dbo::connection TestPtr::db ;



TEST_F(TestPtr, TestPtr) {
	dbo::ptr<kSimpleTable> p ;
	dbo::ptr<kSimpleTable> q ;
	dbo::ptr<kSimpleTable> r ;

	ASSERT_FALSE( p ) ;
	ASSERT_FALSE( q ) ;
	ASSERT_FALSE( r ) ;

	ASSERT_TRUE( p==nullptr ) ;

	p = dbo::make_ptr<kSimpleTable>() ;
	ASSERT_TRUE( (bool)p ) ;
	ASSERT_FALSE( (bool)q ) ;
	ASSERT_FALSE( (bool)r ) ;

	q = p ;
	ASSERT_TRUE( (bool)p ) ;
	ASSERT_TRUE( (bool)q ) ;
	ASSERT_TRUE( p==q ) ;
	ASSERT_FALSE( p==r ) ;

}

TEST_F(TestPtr, TestPtrCopy) {
	dbo::ptr<kSimpleTable> p=dbo::make_ptr<kSimpleTable>() ;
	p->object = dbo::make_ptr<kSimpleTable2>() ;
	ASSERT_TRUE( p->object!=nullptr ) ;

	dbo::ptr<kSimpleTable> q(p) ;
	ASSERT_TRUE( q!=nullptr ) ;
	ASSERT_TRUE( q->object!=nullptr ) ;

	dbo::ptr<kSimpleTable> r ;
	ASSERT_TRUE( r==nullptr ) ;
	r = q ;
	ASSERT_TRUE( r!=nullptr ) ;
	ASSERT_TRUE( r->object!=nullptr ) ;

}

TEST_F(TestPtr, TestWeakPtr) {
	dbo::ptr<kSimpleTable> p=dbo::make_ptr<kSimpleTable>() ;
	dbo::weak_ptr<kSimpleTable> w ;
	dbo::weak_ptr<kSimpleTable> w2 ;

	ASSERT_TRUE( w.expired() ) ;

	w = p ;
	ASSERT_FALSE( w.expired() ) ;

	ASSERT_TRUE( w.lock()==p ) ;

	p.reset() ;
	ASSERT_TRUE( w.expired() ) ;

	w2 = dbo::make_ptr<kSimpleTable>() ;
	ASSERT_TRUE( w.expired() ) ;

}

TEST_F(TestPtr, TestStdPtr) {
	std::shared_ptr<kSimpleTable> p1 ;
	std::shared_ptr<kSimpleTable> p2 ;

	std::weak_ptr<kSimpleTable> w1 ;
	std::weak_ptr<kSimpleTable> w2 ;

	p1 = std::make_shared<kSimpleTable>() ;
	p1->string_value = "toto" ;
	w1 = p1 ;

	p1.reset() ;
	ASSERT_TRUE( w1.lock()==nullptr ) ;
}

TEST_F(TestPtr, DISABLED_TestThreadPtr) {
	dbo::ptr<kSimpleTable> c=dbo::make_ptr<kSimpleTable>() ;
	std::shared_ptr<kSimpleTable> cs=std::make_shared<kSimpleTable>() ;

	typedef std::chrono::duration<double, std::ratio<1>> DurationSeconds ;

	int count1=0 ;
	int count2=0 ;
	auto th_alloc=[&](){
		auto start = std::chrono::high_resolution_clock::now() ;

		while(DurationSeconds(std::chrono::high_resolution_clock::now()-start).count()<10)
		{
//			dbo::ptr<aSimpleTable> e=dbo::make_ptr<aSimpleTable>() ;
//			c = e ;

			std::shared_ptr<kSimpleTable> es=std::make_shared<kSimpleTable>() ;
			cs = es ;

			count1++ ;
		}
	} ;

	auto th_share=[&](){
		auto start = std::chrono::high_resolution_clock::now() ;

		while(DurationSeconds(std::chrono::high_resolution_clock::now()-start).count()<10)
		{
			{
//				dbo::ptr<aSimpleTable> d ;
//				d = c ;

				std::shared_ptr<kSimpleTable> ds ;
				ds = cs ;
			}
			count2++ ;
		}
	} ;

	std::thread th1(th_alloc) ;

	std::vector<std::shared_ptr<std::thread>> ths ;
	for(int i=0 ; i<10 ; i++)
		ths.push_back(std::make_shared<std::thread>(th_share)) ;
	th1.join() ;
	for(auto& th : ths)
		th->join() ;

	std::cout << "count1=" << count1 << "  count2=" << count2 << std::endl ;
}

