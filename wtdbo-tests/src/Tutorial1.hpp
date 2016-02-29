/*
 * Copyright (C) 2010 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

/*****
 * This file is part of the Wt::Dbo tutorial:
 * http://www.webtoolkit.eu/wt/doc/tutorial/dbo/tutorial.html
 *****/

#include <gtest/gtest.h>

#include <iostream>

#include <stdio.h>
#include <stdlib.h>

extern wtdbo::backend::Postgres* db ;

/*****
 * Dbo tutorial section 2. Mapping a single class
 *****/

class T1User {
public:
	enum Role {
		Visitor=0,
		Admin=1,
		Alien=42
	} ;

	std::string name ;
	std::string password ;
	Role        role ;
	int         karma ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, name,     "name") ;
		wtdbo::field(a, password, "password") ;
		wtdbo::field(a, role,     "role") ;
		wtdbo::field(a, karma,    "karma") ;
	}
};

// The fixture for testing class Database.
class Tutorial1 : public ::testing::Test
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


TEST_F(Tutorial1, run) {
	/*****
	 * Dbo tutorial section 3. A first session
	 *****/

	/*
 	 * Setup a session, would typically be done once at application startup.
	 *
	 * For testing, we'll be using Sqlite3's special :memory: database. You
	 * can replace this with an actual filename for actual persistence.
	 */
	wtdbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<T1User>("t1user") ;

	/*
	 * Try to create the schema (will fail if already exists).
	 */
	session.createTables() ;

	{
		wtdbo::Transaction transaction(session) ;

		T1User* user=new T1User() ;
		user->name = "Joe" ;
		user->password = "Secret" ;
		user->role = T1User::Visitor ;
		user->karma = 13 ;

		wtdbo::ptr<T1User> userPtr=session.add(user) ;
	}

	/*****
	 * Dbo tutorial section 4. Querying objects
	 *****/
	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T1User> joe=session.find<T1User>().where("name = ?").bind("Joe") ;

		std::cerr << "Joe has karma: " << joe->karma << std::endl ;

		wtdbo::ptr<T1User> joe2=session.query< wtdbo::ptr<T1User> >("select u from \"t1user\" u").where("name = ?").bind("Joe") ;
	}

	{
		wtdbo::Transaction transaction(session) ;

		typedef wtdbo::collection< wtdbo::ptr<T1User> > Users ;

		Users users=session.find<T1User>() ;

		std::cerr << "We have " << users.size() << " users:" << std::endl ;

		for (Users::const_iterator i=users.begin() ; i != users.end() ; ++i)
			std::cerr << " user " << (*i)->name << " with karma of " << (*i)->karma << std::endl ;
	}

	/*****
	 * Dbo tutorial section 5. Updating objects
	 *****/
	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T1User> joe=session.find<T1User>().where("name = ?").bind("Joe") ;

		joe.modify()->karma++ ;
		joe.modify()->password = "public" ;
	}

	{
		wtdbo::Transaction transaction(session) ;
		wtdbo::ptr<T1User> joe=session.find<T1User>().where("name = ?").bind("Joe") ;
		if(joe)
			joe.remove() ;
	}

	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T1User> silly=session.add(new T1User()) ;
		silly.modify()->name = "Silly" ;
		silly.remove() ;
	}

}
