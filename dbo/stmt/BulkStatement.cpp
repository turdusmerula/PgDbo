#include "dbo/stmt/BulkStatement.h"

#include "dbo/connection.h"

#include <postgresql/libpq-fe.h>

using namespace dbo::stmt ;


BulkStatement::BulkStatement(connection& conn, const std::string& table)
	:	Statement(conn),
		sep_(";"),
		table_(table),
		column_(0),
		row_(0)
{
	buffers_.push_back(Buffer()) ;
}

BulkStatement::~BulkStatement()
{
}

void BulkStatement::bind()
{
	if(column_>0)
		addData(sep_) ;
	if(column_>=columns_.size())
	{
		std::stringstream ss ;
		ss << "BulkStatement bind error: column " << column_ << " undefined" ;
		throw Exception(ss.str()) ;
	}

	addData("\\N") ;
	column_++ ;
}

void BulkStatement::bind(const std::string& value)
{
	if(column_>0)
		addData(sep_) ;
	if(column_>=columns_.size())
	{
		std::stringstream ss ;
		ss << "BulkStatement bind error: column " << column_ << " undefined" ;
		throw Exception(ss.str()) ;
	}

	addData(escape(value)) ;
	column_++ ;
}

void BulkStatement::bind(const std::vector<unsigned char>& value)
{
	if(column_>0)
		addData(sep_) ;
	if(column_>=columns_.size())
	{
		std::stringstream ss ;
		ss << "BulkStatement bind error: column " << column_ << " undefined" ;
		throw Exception(ss.str()) ;
	}
	addData(escapeHex(value)) ;
	column_++ ;
}

bool BulkStatement::read(char*& value)
{
	return false ;
}

bool BulkStatement::read(std::vector<unsigned char>& value)
{
	return false ;
}

void BulkStatement::prepare()
{
	sql_ << "copy \"" << table_ << "\" " << "(" ;

	bool first=true ;
	for(auto& column : columns_)
	{
		if(first==false)
			sql_ << ", " ;
		sql_ << column ;
		first  = false ;
	}

	sql_ << ") from stdin" ;

	sql_ << " with (FORMAT text, " ;
	sql_ << "DELIMITER '" << sep_ << "')" ;
	sql_ << ";" << "\n" ;

}

void BulkStatement::execute()
{
//	sql_ << "\\." ;

	if(conn_->showQueries())
		std::cerr << sql_.str() << std::endl ;

	auto result = std::shared_ptr<pg_result>(PQexec(conn_->conn_, sql_.str().c_str()), pg_result_deleter) ;
	auto res=PQresultStatus(result.get()) ;
	if(res!=PGRES_COPY_IN)
	{
		std::stringstream ss ;
		ss << "BulkStatement execution failed: " << PQerrorMessage(conn_->conn_) ;
		ss << " -> " << sql_.str() ;
		throw Exception(ss.str()) ;
	}

	for(auto& buff : buffers_)
	{
		if(buff.size_>0)
		{
			if(PQputCopyData(conn_->conn_, reinterpret_cast<const char*>(buff.data_), buff.size_)==-1)
			{
				std::stringstream ss ;
				ss << "BulkStatement execution failed: " << PQerrorMessage(conn_->conn_) ;
				ss << " -> " << sql_.str() ;
				throw Exception(ss.str()) ;
			}
		}
	}

	auto endres=PQputCopyEnd(conn_->conn_, nullptr) ;
	if(endres==0 || endres==-1)
	{
		std::stringstream ss ;
		ss << "BulkStatement execution failed: " << PQerrorMessage(conn_->conn_) ;
		ss << " -> " << sql_.str() ;
		throw Exception(ss.str()) ;
	}

	auto dataresult = std::shared_ptr<pg_result>(PQgetResult(conn_->conn_), pg_result_deleter) ;
	size_t inserted=0 ;
	try {
		inserted = boost::lexical_cast<size_t>(PQcmdTuples(dataresult.get())) ;
	} catch(...) {

	}
	if(inserted!=row_)
	{
		std::stringstream ss ;
		ss << "BulkStatement execution failed: tried to insert " << row_ << " rows, " << inserted << " inserted" ;
		ss << " -> " << sql_.str() ;
		throw Exception(ss.str()) ;
	}
}

std::string BulkStatement::escape(const std::string& value)
{
	std::string res ;

	res.reserve(value.size()) ;
	for(auto c : value)
	{
		switch(c)
		{
		case '\b':
			res += "\\b" ;
			break ;
		case '\f':
			res += "\\f" ;
			break ;
		case '\n':
			res += "\\n" ;
			break ;
		case '\r':
			res += "\\r" ;
			break ;
		case '\t':
			res += "\\t" ;
			break ;
		case '\v':
			res += "\\v" ;
			break ;
		case '\\':
			res += "\\\\" ;
			break ;
		case ';':
			res += "\\073" ;
			break ;
		default:
			res += c ;
		}
	}

	return res ;
}

std::string BulkStatement::escapeHex(const std::vector<unsigned char>& value)
{
	std::string res ;

	res = "\\\\x" ;

	res.reserve(value.size()*3) ;
	for(auto c : value)
	{
		int h=(c>>4) ;
		int l=(c&0x0F) ;
		res += (h<10?'0'+h:'A'+h-10) ;
		res += (l<10?'0'+l:'A'+l-10) ;
	}

	return res ;
}

void BulkStatement::addColumn(const std::string& name)
{
	columns_.push_back(name) ;
}

bool BulkStatement::nextRow()
{
	if(column_<columns_.size()-1)
	{
		std::stringstream ss ;
		ss << "BulkStatement preparation failed: not all columns bound for row " << row_ ;
		ss << " -> " << sql_.str() ;
		throw Exception(ss.str()) ;
	}

	column_ = 0 ;
	row_++ ;
	addData("\n") ;
	return false ;
}

void BulkStatement::pg_result_deleter(pg_result* result)
{
	PQclear(result) ;
	result = nullptr ;
}

void BulkStatement::addData(const char* data)
{
	size_t len=strlen(data) ;
	char* str=const_cast<char*>(data) ;

	while(len>0)
	{
		Buffer* buff=&buffers_.back() ;
		size_t freesize=BuffSize-buff->size_ ;

		if(freesize==0)
		{
			buffers_.push_back(Buffer()) ;
			buff = &buffers_.back() ;
			freesize = BuffSize ;
		}

		size_t cpysize ;
		if(len<freesize)
			cpysize = len ;
		else
			cpysize = freesize ;

		memcpy(buff->data_+buff->size_, str, cpysize) ;
		len -= cpysize ;
		str += cpysize ;
		buff->size_ += cpysize ;
	}
}

void BulkStatement::addData(const std::string& data)
{
	addData(data.c_str()) ;
}
