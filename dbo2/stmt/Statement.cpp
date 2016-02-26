#include "dbo2/stmt/Statement.h"

#include "dbo2/connection.h"

using namespace dbo2::stmt ;


Statement::Statement(connection& conn)
	:	conn_(&conn)
{
}

Statement::~Statement()
{
}

