#ifndef _DBO_DBO_HPP_
#define _DBO_DBO_HPP_

#include <csignal>

#include <dbo/connection.h>
#include <dbo/ptr.hpp>
#include <dbo/ref.hpp>
#include <dbo/collection.hpp>
#include <dbo/relation.hpp>
#include <dbo/query.hpp>
#include <dbo/Exception.h>

#include <dbo/stmt/BulkStatement.h>
#include <dbo/stmt/PreparedStatement.h>

#include <dbo/traits/dbo_traits.hpp>
#include <dbo/traits/StdSqlTraits.h>

#include <dbo/mapping/Mapping.hpp>
#include <dbo/mapping/CollectionRef.hpp>
#include <dbo/mapping/FieldRef.hpp>
#include <dbo/mapping/PtrRef.hpp>
#include <dbo/mapping/RefRef.hpp>

#include <dbo/action/InitSchema.hpp>
#include <dbo/action/Delete.hpp>
#include <dbo/action/BulkInsert.hpp>
#include <dbo/action/Insert.hpp>
#include <dbo/action/Update.hpp>
#include <dbo/action/SelectById.hpp>
#include <dbo/action/LoadDb.hpp>

#include <dbo/traits/dbo_traits.hpp>



#include <dbo/connection.cxx>
#include <dbo/ptr.cxx>
#include <dbo/ref.cxx>
#include <dbo/collection.cxx>
#include <dbo/relation.cxx>
#include <dbo/query.cxx>

#include <dbo/action/InitSchema.cxx>
#include <dbo/action/Delete.cxx>
#include <dbo/action/BulkInsert.cxx>
#include <dbo/action/Insert.cxx>
#include <dbo/action/Update.cxx>
#include <dbo/action/SelectById.cxx>
#include <dbo/action/LoadDb.cxx>

#include <dbo/mapping/FieldRef.cxx>
#include <dbo/mapping/PtrRef.cxx>
#include <dbo/mapping/RefRef.cxx>
#include <dbo/mapping/Mapping.cxx>

#endif
