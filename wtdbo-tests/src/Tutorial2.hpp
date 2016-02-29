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
 * Dbo tutorial section 6. Mapping relations
 *****/

class T2Post ;
class T2User ;
class T2Tag ;

class T2Post
{
public:
	wtdbo::ptr<T2User> user ;
	wtdbo::collection< wtdbo::ptr<T2Tag> > tags ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::belongsTo(a, user, "user") ;
		wtdbo::hasMany(a, tags, wtdbo::ManyToMany, "post_tags") ;
	}
} ;

class T2Tag
{
public:
	std::string name ;
	wtdbo::collection< wtdbo::ptr<T2Post> > posts ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, name, "name") ;
		wtdbo::hasMany(a, posts, wtdbo::ManyToMany, "post_tags") ;
	}
} ;

class T2Settings
{
public:
	std::string theme ;

	wtdbo::ptr<T2User> user ;

	template<class Action>
	void persist(Action& a)
	{
		wtdbo::field(a, theme, "theme") ;
		wtdbo::belongsTo(a, user) ;
	}
} ;

class T2User
{
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

  wtdbo::collection< wtdbo::ptr<T2Post> > posts ;
  wtdbo::weak_ptr<T2Settings> settings ;

  	  template<class Action>
  	  void persist(Action& a)
  	  {
  		  wtdbo::field(a, name,     "name") ;
  		  wtdbo::field(a, password, "password") ;
  		  wtdbo::field(a, role,     "role") ;
  		  wtdbo::field(a, karma,    "karma") ;

  		  wtdbo::hasMany(a, posts, wtdbo::ManyToOne, "user") ;
  		  wtdbo::hasOne(a, settings) ;
  	  }
} ;


// The fixture for testing class Database.
class Tutorial2 : public ::testing::Test
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

TEST_F(Tutorial2, run) {
	/*
	 * Setup a session, would typically be done once at application startup.
	 */
	wtdbo::Session session ;
	session.setConnection(*db) ;

	session.mapClass<T2User>("t2user") ;
	session.mapClass<T2Post>("t2post") ;
	session.mapClass<T2Tag>("t2tag") ;
	session.mapClass<T2Settings>("t2settings") ;

	/*
	 * Try to create the schema (will fail if already exists).
	 */
	session.createTables() ;

	{
		wtdbo::Transaction transaction(session) ;

		T2User* user=new T2User();
		user->name = "Joe" ;
		user->password = "Secret" ;
		user->role = T2User::Visitor ;
		user->karma = 13 ;

		wtdbo::ptr<T2User> userPtr=session.add(user) ;
	}

	wtdbo::ptr<T2Post> post ;
	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T2User> joe=session.find<T2User>().where("name = ?").bind("Joe") ;

		post = session.add(new T2Post()) ;
		post.modify()->user = joe ;

		// will print 'Joe has 1 post(s).'
		std::cerr << "Joe has " << joe->posts.size() << " post(s)." << std::endl ;
	}

	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T2Tag> cooking = session.add(new T2Tag()) ;
		cooking.modify()->name = "Cooking" ;

		post.modify()->tags.insert(cooking) ;

		// will print '1 post(s) tagged with Cooking.'
		std::cerr << cooking->posts.size() << " post(s) tagged with Cooking." << std::endl ;
	}

	{
		wtdbo::Transaction transaction(session) ;

		wtdbo::ptr<T2User> joe = session.find<T2User>().where("name = ?").bind("Joe") ;

		wtdbo::ptr<T2Settings> settings = session.add(new T2Settings()) ;
		settings.modify()->theme = "fancy-pink" ;
		joe.modify()->settings = settings ;

		// will print 'Settings apply to Joe.'
		std::cerr << "Settings apply to " << settings->user->name << std::endl ;
	}
}
