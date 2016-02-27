/*
 * Copyright (C) 2010 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <wtdbo/FixedSqlConnectionPool.h>
#include <wtdbo/SqlConnection.h>

#include <boost/thread.hpp>
#include <boost/thread/condition.hpp>

namespace wtdbo
{

struct FixedSqlConnectionPool::Impl
{
	boost::mutex mutex;
	boost::condition connectionAvailable;

	std::vector<SqlConnection *> freeList;
};

FixedSqlConnectionPool::FixedSqlConnectionPool(SqlConnection *connection, int size)
{
	impl_ = new Impl();

	impl_->freeList.push_back(connection);

	for(int i = 1 ; i<size ; ++i)
		impl_->freeList.push_back(connection->clone());
}

FixedSqlConnectionPool::~FixedSqlConnectionPool()
{
	for(unsigned i = 0 ; i<impl_->freeList.size() ; ++i)
		delete impl_->freeList[i];

	delete impl_;
}

SqlConnection *FixedSqlConnectionPool::getConnection()
{
	boost::mutex::scoped_lock lock(impl_->mutex);

	while(impl_->freeList.empty())
		impl_->connectionAvailable.wait(impl_->mutex);

	SqlConnection *result = impl_->freeList.back();
	impl_->freeList.pop_back();

	return result;
}

void FixedSqlConnectionPool::returnConnection(SqlConnection *connection)
{
	boost::mutex::scoped_lock lock(impl_->mutex);

	impl_->freeList.push_back(connection);

	if(impl_->freeList.size()==1)
		impl_->connectionAvailable.notify_one();
}

void FixedSqlConnectionPool::prepareForDropTables() const
{
	for(unsigned i = 0 ; i<impl_->freeList.size() ; ++i)
		impl_->freeList[i]->prepareForDropTables();
}

}
