#ifndef _DBO_DBO_HPP_
#define _DBO_DBO_HPP_

#include <csignal>

#include <dbo2/connection.h>
#include <dbo2/ptr.hpp>
#include <dbo2/collection.hpp>
#include <dbo2/relation.hpp>
#include <dbo2/query.hpp>
#include <dbo2/Exception.h>

#include <dbo2/stmt/Statement.h>

#include <dbo2/traits/dbo_traits.hpp>
#include <dbo2/traits/StdSqlTraits.h>

#include <dbo2/mapping/Mapping.hpp>
#include <dbo2/mapping/FieldRef.hpp>
#include <dbo2/mapping/PtrRef.hpp>

#include <dbo2/action/InitSchema.hpp>
#include <dbo2/action/Delete.hpp>
#include <dbo2/action/Insert.hpp>
#include <dbo2/action/Update.hpp>
#include <dbo2/action/SelectById.hpp>
#include <dbo2/action/LoadDb.hpp>

#include <dbo2/traits/dbo_traits.hpp>



#include <dbo2/connection.cxx>
#include <dbo2/ptr.cxx>
#include <dbo2/collection.cxx>
#include <dbo2/relation.cxx>
#include <dbo2/query.cxx>

#include <dbo2/action/InitSchema.cxx>
#include <dbo2/action/Delete.cxx>
#include <dbo2/action/Insert.cxx>
#include <dbo2/action/Update.cxx>
#include <dbo2/action/SelectById.cxx>
#include <dbo2/action/LoadDb.cxx>

#include <dbo2/mapping/FieldRef.cxx>
#include <dbo2/mapping/PtrRef.cxx>
#include <dbo2/mapping/Mapping.cxx>

#endif
