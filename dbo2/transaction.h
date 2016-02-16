#ifndef _DBO_TRANSACTION_H_
#define _DBO_TRANSACTION_H_

namespace dbo2 {
class connection ;

class transaction
{
public:
	/*! \brief Destructor.
	 *
	 * If the transaction is still active, it is rolled back.
	 */
	~transaction() noexcept(false) ;

	/*! \brief Returns whether the transaction is still active.
	 *
	 * A transaction is active unless it has been committed or rolled
	 * back.
	 *
	 * While a transaction is active, new transactions for the same
	 * session are treated as the same transaction. Nested transactions
	 * are not implemented.
	 */
	bool active() const ;

	/*! \brief Commits the transaction.
	 *
	 * \sa rollback()
	 */
	void commit() ;

	/*! \brief Rolls back the transaction.
	 *
	 * \sa commit()
	 */
	void rollback() ;

protected:
	/*! \brief Constructor.
	 *
	 * Opens a transaction for the given \p session. If a transaction is
	 * already open for the connection nothing is done.
	 */
	 transaction(connection& conn) ;

	 void open() ;

private:
	connection& conn_ ;
	bool active_ ;

//	std::deque<ptr_base> transiants_ ;

	friend connection ;
} ;

}

#endif
