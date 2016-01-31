#include <iostream>
#include <boost/thread.hpp>
#include <chrono>

#include <data/PgSession.h>
#include <data/object/OutMail.h>

const int numThread=30 ;

boost::mutex mutex_ ;

using namespace data ;

void ThreadFunction()
{
    int counter=0 ;
    int errors=0 ;

    // one session by thread
	data::PgSession db ;

    auto start = std::chrono::high_resolution_clock::now() ;

	for(;;)
	{
		if(boost::this_thread::interruption_requested())
		{
			boost::mutex::scoped_lock lock(mutex_) ;

			// do something time-consuming
			auto end=std::chrono::high_resolution_clock::now() ;
			double timeTakenInSeconds=(end - start).count()*((double) std::chrono::high_resolution_clock::period::num/std::chrono::high_resolution_clock::period::den) ;

			std::cout << counter << " iterations in " << timeTakenInSeconds << " seconds, " << errors << " errors" << std::endl ;

			return ;
		}


		try {
			auto& tn=PgSession::transaction() ;

			// add a new session
			common::Mail mail("me@gmail.com", "test", "test") ;
			data::OutMail* bdmail=new data::OutMail(mail) ;
			tn.session().add(bdmail) ;

			tn.commit() ;
		} catch(std::exception& e) {
			errors++ ;
			//std::cerr << "Error: " << e.what() << std::endl ;
			//return ;
		}

		counter++ ;
	}
}

int main()
{
    // Start thread
    boost::thread* threads[numThread] ;

    //create test user
	try {
    	// instanciate database so it can perfom one time initialisations
    	PgSession db ;

		auto& tn=PgSession::transaction() ;

		if(data::user::User::exist("a.a@a.com")==false)
		{
			data::user::User* user=new data::user::User ;
			user->create("a.a@a.com", "aaaaa") ;
			tn.session().add(user) ;
		}

		tn.commit() ;
	} catch(std::exception& e) {
		std::cerr << "Error: " << e.what() << std::endl ;
		return 1 ;
	}

    for(auto& t : threads)
    	t = new boost::thread(&ThreadFunction) ;

    // Wait for Enter
    char ch;
    std::cin.get(ch) ;

    // Ask thread to stop
    for(auto& t : threads)
    	t->interrupt() ;

    // Join - wait when thread actually exits
    for(auto& t : threads)
    	t->join() ;

    return 0;
}
