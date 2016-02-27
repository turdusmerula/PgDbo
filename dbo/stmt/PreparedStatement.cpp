#include "dbo/stmt/PreparedStatement.h"

#include "dbo/connection.h"

#include <boost/lexical_cast.hpp>

#include <iostream>

#include <postgresql/libpq-fe.h>

using namespace dbo::stmt ;


PreparedStatement::PreparedStatement(connection& conn, std::string name, std::string sql)
	:	Statement(conn),
		prepared_(false),
		row_(-1),
		affectedRows_(0),
		column_(0),
		name_(name),
		hashname_(false),
		rawsql_(sql)
{
}

PreparedStatement::PreparedStatement(connection& conn, std::string sql)
	:	Statement(conn),
		prepared_(false),
		row_(-1),
		affectedRows_(0),
		column_(0),
		hashname_(true),
		rawsql_(sql)
{
}

//PreparedStatement::PreparedStatement(const PreparedStatement& other)
//	:	conn_(other.conn_)
//{
//}

PreparedStatement::~PreparedStatement()
{
}

//PreparedStatement& operator=(const PreparedStatement& other)
//{
//	if(other.result_==nullptr)
//	{
//		std::stringstream ss ;
//		ss << "PreparedStatement '" << name_ << "' error: statements with a request result cannot be copied" ;
//		ss << " -> " << sql_ ;
//		throw Exception(ss.str()) ;
//	}
//
//	if(conn_!=other.conn_)
//	{
//		std::stringstream ss ;
//		ss << "PreparedStatement '" << name_ << "' error: assignment needs statements from the same connections" ;
//		ss << " -> " << sql_ ;
//		throw Exception(ss.str()) ;
//	}
//
//	hashname_ = other.hashname_ ;
//	name_ = other.name_ ;
//
//	rawsql_ = other.rawsql_ ;
//	sql_ = other.sql_ ;
//
//	prepared_ = other.prepared_ ;
//	paramCount_ = other.paramCount_ ;
//
//	result_ = nullptr ;
//	row_ = -1 ;
//	column_ = 0 ;
//	affectedRows_ =0 ;
//	hasReturning_ = other.hasReturning_ ;
//
//	oids_ = other.oids_ ;
//
//	svalues_ = other.svalues_ ;
//	values_ = other.values_ ;
//	lengths_ = other.lengths_ ;
//	formats_ = other.formats_ ;
//
//	return *this ;
//}

void PreparedStatement::bind()
{
	if(prepared_==false)
		oids_.push_back(OIDDefault) ;

	svalues_.push_back("") ;
	values_.push_back(nullptr) ;
	lengths_.push_back(0) ;
	formats_.push_back(0) ;
}

void PreparedStatement::bind(const std::string& value)
{
	if(prepared_==false)
		oids_.push_back(OIDDefault) ;

	svalues_.push_back(value) ;
	values_.push_back(svalues_.back().c_str()) ;
	lengths_.push_back(0) ;
	formats_.push_back(0) ;
}

void PreparedStatement::bind(const std::vector<unsigned char>& value)
{
	if(prepared_==false)
		oids_.push_back(OIDBytea) ;

	std::string p ;
	p.resize(value.size()) ;
	if(value.size()>0)
		memcpy(const_cast<char *>(p.data()), value.data(), value.size()) ;

	svalues_.push_back(p) ;
	values_.push_back(svalues_.back().c_str()) ;
	lengths_.push_back(value.size()) ;
	formats_.push_back(1) ;
}

bool PreparedStatement::read(char*& value)
{
	if(PQgetisnull(result_.get(), row_, column_))
	{
		column_++ ;
		return false ;
	}

	value = PQgetvalue(result_.get(), row_, column_) ;

	column_++ ;
	return true ;
}

bool PreparedStatement::read(std::vector<unsigned char>& value)
{
	if(PQgetisnull(result_.get(), row_, column_))
	{
		column_++ ;
		return false ;
	}

	const char* escaped=PQgetvalue(result_.get(), row_, column_) ;

	std::size_t vlength ;
	unsigned char* v=PQunescapeBytea((unsigned char *)escaped, &vlength) ;

	value.resize(vlength) ;
	std::copy(v, v+vlength, value.begin()) ;
	PQfreemem(v) ;

	column_++ ;
	return true ;
}

void PreparedStatement::prepare()
{
	if(conn_->connected()==false)
		Exception("Database not connected") ;

    sql_ = convertToNumberedPlaceholders(rawsql_) ;
	paramCount_ = getNumberPlaceHolders(sql_) ;

	const std::string returning=" returning " ;
	std::size_t j=sql_.rfind(returning) ;
	if(j!=std::string::npos && sql_.find(' ', j+returning.length())==std::string::npos)
		hasReturning_ = true ;
	else
		hasReturning_ = false ;

	if(oids_.size()!=paramCount_)
	{
		std::stringstream ss ;
		ss << "PreparedStatement '" << name_ << "' not properly prepared: " << oids_.size() << " types provided, " << paramCount_ << " needed" ;
		ss << " -> " << sql_ ;
		throw Exception(ss.str()) ;
	}

	if(hashname_)
	{
		name_ = boost::lexical_cast<std::string>(std::hash<std::string>{}(sql_)) ;

		// In case name is a hash it is ok to reuse a previous statement as we know it will be the same
		auto result=std::shared_ptr<pg_result>(PQdescribePrepared(conn_->conn_, name_.c_str()), pg_result_deleter) ;
		auto err=PQresultStatus(result.get()) ;
		if(err==PGRES_COMMAND_OK)
		{
			// PreparedStatement already exist,exit ok
			prepared_ = true ;
			return ;
		}
	}

	auto result=std::shared_ptr<pg_result>(PQprepare(conn_->conn_, name_.c_str(), sql_.c_str(), oids_.size(), (Oid *)oids_.data()), pg_result_deleter) ;
	auto err=PQresultStatus(result.get()) ;
	if(err!=PGRES_COMMAND_OK && err!=PGRES_TUPLES_OK)
	{
		std::stringstream ss ;
		ss << "PreparedStatement '" << name_ << "' preparation failed: " << PQerrorMessage(conn_->conn_) << " needed" ;
		ss << " -> " << sql_ ;
		throw Exception(ss.str()) ;
	}

	prepared_ = true ;
}

void PreparedStatement::reset()
{
	values_.clear() ;
	svalues_.clear() ;
	lengths_.clear() ;
	formats_.clear() ;

	row_ = -1 ;
	column_ = 0 ;
	affectedRows_ = 0 ;

	result_.reset() ;
}

void PreparedStatement::execute()
{
	if(svalues_.size()!=paramCount_)
	{
		std::stringstream ss ;
		ss << "PreparedStatement '" << name_ << "' was not bound correctly: " << svalues_.size() << " bindings provided, " << paramCount_ << " needed" ;
		ss << " -> " << sql_ ;
		throw Exception(ss.str()) ;
	}

	if(conn_->showQueries() && conn_->showBindings())
		std::cerr << sql_ << " -> (" << getBoundPlaceholders() << ")" << std::endl ;
	else if(conn_->showQueries())
		std::cerr << sql_ << std::endl ;

	result_.reset() ;

	// put result in a shared_ptr with a custom deleter to clear it
	result_ = std::shared_ptr<pg_result>(PQexecPrepared(conn_->conn_, name_.c_str(), oids_.size(), values_.data(), lengths_.data(), formats_.data(), 0), pg_result_deleter) ;
	auto res=PQresultStatus(result_.get()) ;

	if(res==PGRES_COMMAND_OK)
	{
		std::string s = PQcmdTuples(result_.get()) ;
		if(!s.empty())
			affectedRows_ = boost::lexical_cast<int>(s) ;
		else
			affectedRows_ = 0 ;
	}
	else if(res==PGRES_TUPLES_OK)
		affectedRows_ = PQntuples(result_.get()) ;

	if(conn_->showResults())
	{
		if(res!=PGRES_COMMAND_OK && res!=PGRES_TUPLES_OK)
			std::cerr << "Error executing request" << std::endl ;
		else
		{
			std::cerr << "Fetched " << affectedRows_ << " lines" << std::endl ;
			for(int i=0 ; i<affectedRows_ ; i++)
				// TODO show result lines
				;
		}
	}

	if(res!=PGRES_COMMAND_OK && res!=PGRES_TUPLES_OK)
	{
		result_.reset() ;

		std::stringstream ss ;
		ss << "PreparedStatement '" << name_ << "' execution failed: " << PQerrorMessage(conn_->conn_) ;
		ss << " -> " << sql_ ;
		throw Exception(ss.str()) ;
	}
}

bool PreparedStatement::nextRow()
{
	if(row_+1<PQntuples(result_.get()))
	{
		row_++ ;
		column_ = 0 ;
		return true ;
	}
	return false ;
}

std::string PreparedStatement::convertToNumberedPlaceholders(const std::string& sql)
{
	std::stringstream result ;
	std::map<std::string, std::string> placeholders ;

	getPlaceHolders(sql, placeholders) ;

	enum
	{
		PreparedStatement,
		SQuote,
		DQuote
	} state = PreparedStatement ;

	int placeholder=1 ;

	for(unsigned i=0 ; i<sql.length() ; ++i)
	{
		switch(state)
		{
		case PreparedStatement:
			if(sql[i]=='\'')
				state = SQuote ;
			else if(sql[i]=='"')
				state = DQuote ;
			else if(sql[i]=='?')
			{
				// search for a free placeholder number
				std::string ph="$"+boost::lexical_cast<std::string>(placeholder) ;
				while(placeholders.find(ph)!=placeholders.end())
					ph = "$"+boost::lexical_cast<std::string>(++placeholder) ;
				result <<  ph ;
				placeholders[ph] = ph ;
				continue ;
			}
			break ;
		case SQuote:
			if(sql[i]=='\'')
			{
				if(i+1==sql.length())
					state = PreparedStatement ;
				else if(sql[i+1]=='\'')
				{
					result << sql[i] ;
					++i ; // skip to next
				}
				else
					state = PreparedStatement ;
			}
			break ;
		case DQuote:
			if(sql[i]=='"')
				state = PreparedStatement ;
			break ;
		}
		result << sql[i] ;
	}

	return result.str() ;
}

size_t PreparedStatement::getNumberPlaceHolders(const std::string& sql)
{
	std::map<std::string, std::string> placeholders ;

	return getPlaceHolders(sql, placeholders) ;
}

size_t PreparedStatement::getPlaceHolders(const std::string& sql, std::map<std::string, std::string>& placeholders)
{
	for(size_t i=0 ; i<sql.length() ; ++i)
	{
		if(sql[i]=='$')
		{
			std::string placeholder="$" ;
			for(size_t j=i+1 ; j<sql.length() && isdigit(sql[j]) ; j++)
			{
				if(isdigit(sql[j]))
				{
					placeholder.push_back(sql[j]) ;
					i++ ;
				}
			}

			if(placeholder.size()>1)
				placeholders[placeholder] = placeholder ;
		}
	}

	return placeholders.size() ;
}

std::string PreparedStatement::getBoundPlaceholders()
{
	std::stringstream ss ;

	for(int i=0 ; i<oids_.size() ; i++)
	{
		if(i>0)
			ss << ", " ;
		ss << "$" << i+1 << "=" ;
		if(i<svalues_.size())
		{
			if(oids_[i]==OIDBytea)
				ss << "blob " << lengths_[i] << " bytes" ;
			else if(values_[i])
				ss << "'" << svalues_[i] << "'" ;
			else
				ss << "null" ;
		}
		else
			ss << "####" ;
	}

	return ss.str() ;
}

void PreparedStatement::sql(const std::string& sql)
{
	if(prepared_)
	{
		std::stringstream ss ;
		ss << "PreparedStatement '" << name_ << "' already prepared, SQL can not be modified"  ;
		throw Exception(ss.str()) ;
	}

	rawsql_ = sql ;
}

void PreparedStatement::name(const std::string& name)
{
	hashname_ = false ;
	name_ = name ;
}

void PreparedStatement::hashName(bool value)
{
	hashname_ = true ;
	name_ = "" ;
}

void PreparedStatement::pg_result_deleter(pg_result* result)
{
	PQclear(result) ;
	result = nullptr ;
}


