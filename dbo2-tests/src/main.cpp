#include <iostream>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <dbo2-tests/src/TestSimpleTable.hpp>
#include <dbo2-tests/src/TestCustomIdTable.hpp>
#include <dbo2-tests/src/TestCompositeIdTable.hpp>
#include <dbo2-tests/src/TestBelongsToTable.hpp>
//#include <dbo2-tests/src/TestHasManyTable.hpp>
//#include <dbo2-tests/src/TestRequest.hpp>

std::string connection ;
dbo2::connection db ;

int main(int argc, char* argv[])
{
	std::string host="localhost" ;
	std::string user="dbotest" ;
	std::string password="dbotest" ;
	std::string port="5432" ;
	std::string dbname="dbotest" ;

	// decode command line
	boost::program_options::options_description desc("Options") ;
	try {
		// parse command line
	    desc.add_options()
	      ("help", "Print help messages")
	      ("host,h", boost::program_options::value<std::string>(), "database host name (default localhost)")
	      ("user,u", boost::program_options::value<std::string>(), "database user name (default dbotest)")
	      ("password,w", boost::program_options::value<std::string>(), "database password (default dbotest)")
	      ("port,p", boost::program_options::value<std::string>(), "database port (default 5432)")
	      ("dbname,d", boost::program_options::value<std::string>(), "database name (default dbotest)")
		  ;

	    boost::program_options::variables_map vm ;
	    boost::program_options::store(boost::program_options::parse_command_line(argc, argv, desc), vm) ;

	    if(vm.count("host"))
	    	host = vm["host"].as<std::string>() ;

	    if(vm.count("user"))
	    	host = vm["user"].as<std::string>() ;

	    if(vm.count("password"))
	    	host = vm["password"].as<std::string>() ;

	    if(vm.count("port"))
	    	host = vm["port"].as<std::string>() ;

	    if(vm.count("dbname"))
	    	host = vm["dbname"].as<std::string>() ;

	    if(vm.count("help")){
	        std::cout << desc ;
	        exit(1) ;
	    }

	} catch(std::exception& e) {
		std::cerr << "Usage " << desc << std::endl ;
		::testing::InitGoogleTest(&argc, argv) ;
		return 1 ;
	}

	connection = "host="+host+" user="+user+" password="+password+" port="+port+" dbname="+dbname ;

	db.connect(connection) ;
	db.showQueries(true) ;
	db.showBindings(true) ;

	::testing::InitGoogleTest(&argc, argv) ;
	return RUN_ALL_TESTS() ;
}
