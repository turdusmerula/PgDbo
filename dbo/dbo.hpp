#ifndef _DBO_DBO_HPP_
#define _DBO_DBO_HPP_

#include <csignal>

#include <dbo/dbo_types.h>

#include <dbo/debug.h>

#include <dbo/connection.cxx>
#include <dbo/ptr.cxx>
#include <dbo/weak_ptr.cxx>
#include <dbo/lazy_ptr.cxx>
#include <dbo/collection.cxx>
#include <dbo/relation.cxx>
#include <dbo/query.cxx>

#include <dbo/action/Debug.cxx>
#include <dbo/action/InitSchema.cxx>
#include <dbo/action/Delete.cxx>
#include <dbo/action/BulkInsert.cxx>
#include <dbo/action/Insert.cxx>
#include <dbo/action/InsertRelation.cxx>
#include <dbo/action/InsertCollection.cxx>
#include <dbo/action/Update.cxx>
#include <dbo/action/UpdateRelation.cxx>
#include <dbo/action/SqlUpdateRelation.cxx>
#include <dbo/action/SelectById.cxx>
#include <dbo/action/LoadDb.cxx>
#include <dbo/action/LoadId.cxx>
#include <dbo/action/SqlInsert.cxx>
#include <dbo/action/SqlInsertRelation.cxx>
#include <dbo/action/SqlSelect.cxx>
#include <dbo/action/SqlSelectCollection.cxx>
#include <dbo/action/SqlUpdate.cxx>

#include <dbo/mapping/FieldRef.cxx>
#include <dbo/mapping/PtrRef.cxx>
#include <dbo/mapping/WeakRef.cxx>
#include <dbo/mapping/Mapping.cxx>

#endif
