#include <iostream>
#include <fstream>

#include <gtest/gtest.h>
#include <boost/filesystem.hpp>
#include <boost/program_options.hpp>

#include <dbo/Dbo.h>
#include <dbo/FixedSqlConnectionPool.h>
#include <dbo/backend/Postgres.h>

#include "TestSimpleTable.hpp"
#include "TestCustomIdTable.hpp"
#include "TestCompositeIdTable.hpp"
#include "TestBelongsToTable.hpp"
#include "TestHasManyTable.hpp"
#include "TestRequest.hpp"

dbo::backend::Postgres* db=nullptr ;
std::unique_ptr<dbo::FixedSqlConnectionPool> pool ;

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

	std::string connection="host="+host+" user="+user+" password="+password+" port="+port+" dbname="+dbname ;

	// prepare database for tests
	db = new dbo::backend::Postgres(connection) ;
	db->setProperty("show-queries", "true") ;
	pool = std::unique_ptr<dbo::FixedSqlConnectionPool>(new dbo::FixedSqlConnectionPool(db, 40)) ;

	::testing::InitGoogleTest(&argc, argv) ;
	return RUN_ALL_TESTS() ;
}
