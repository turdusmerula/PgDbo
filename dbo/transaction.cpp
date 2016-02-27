#include <dbo/transaction.h>
#include <dbo/connection.h>

using namespace dbo ;

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
	active_ = false ;
}

void transaction::rollback()
{
	if(!active_)
		return ;

	conn_.executeSql("ROLLBACK") ;
	active_ = false ;
}

void transaction::open()
{
	if(active_)
		return ;

	conn_.executeSql("BEGIN TRANSACTION") ;
	active_ = true ;
}
