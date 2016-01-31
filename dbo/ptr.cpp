// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include <dbo/ptr.h>
#include <dbo/Exception.h>
#include <dbo/Session.h>

namespace dbo
{

MetaDboBase::~MetaDboBase()
{
}

void MetaDboBase::transactionDone(bool success)
{
	doTransactionDone(success);
}

void MetaDboBase::incRef()
{
	++refCount_;
}

void MetaDboBase::decRef()
{
	--refCount_;

	if(refCount_==0)
		delete this;
}

void MetaDboBase::setState(State state)
{
	state_ &= ~0xF;
	state_ |= state;
}

void MetaDboBase::setDirty()
{
	checkNotOrphaned();
	if(isDeleted())
	{
		return;
	}

	if(!isDirty())
	{
		state_ |= NeedsSave;
		if(Session::current())
			Session::current()->needsFlush(this);
	}
}

void MetaDboBase::remove()
{
	checkNotOrphaned();

	if(isDeleted())
	{
		// is already removed or being removed in this transaction
	}
	else if(isPersisted())
	{
		state_ |= NeedsDelete;
		Session::current()->needsFlush(this);
	}
	else if(Session::current())
	{ // was added to a Session but not yet flushed
		Session::current()->discardChanges(this);
		state_ &= ~NeedsSave;
	}
	else
	{
		// is not yet added to the Session
	}
}

void MetaDboBase::setTransactionState(State state)
{
	state_ &= ~Saving;
	state_ |= state;
}

void MetaDboBase::resetTransactionState()
{
	state_ &= ~TransactionState;
}

void MetaDboBase::checkNotOrphaned()
{
	if(isOrphaned())
	{
		throw Exception("using orphaned dbo ptr");
	}
}

ptr_base::~ptr_base()
{
}

}
