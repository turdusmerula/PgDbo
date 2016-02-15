#include <dbo2/transaction.h>
#include <dbo2/connection.h>

using namespace dbo2 ;

transaction::transaction(connection& conn)
	:	conn_(conn),
		active_(false)
{
}

transaction::~transaction() noexcept(false)
{
	rollback() ;
}

bool transaction::active() const
{
	return active_ ;
}

void transaction::commit()
{
	if(!active_)
		return ;

	conn_.executeSql("COMMIT") ;
}

void transaction::rollback()
{
	if(!active_)
		return ;

	conn_.executeSql("ROLLBACK") ;
}

void transaction::open()
{
	if(active_)
		return ;

	conn_.executeSql("BEGIN TRANSACTION") ;
	active_ = true ;
}
