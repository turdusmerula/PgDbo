#include <dbo/dbo_types.h>
#include "dbo/stmt/Statement.h"

#include "dbo/connection.h"

using namespace dbo::stmt ;


Statement::Statement(connection& conn)
	:	conn_(&conn)
{
}

Statement::~Statement()
{
}

