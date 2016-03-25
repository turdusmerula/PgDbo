#ifndef _DBO_DBO_TYPES_H_
#define _DBO_DBO_TYPES_H_

namespace dbo {
	template <class T> class ptr ;
	template <class T> class weak_ptr ;
	template <class T> class collection ;
	class query ;
	class connection ;

	namespace action {
		template <class T> class LoadDb ;
		template <class T> class LoadId ;
		template <class T> class Delete ;
		template <class T, class U> class Insert ;
		template <class T> class Update ;
		template <class T> class SelectById ;
		template <class T> class SqlInsert ;
		template <class T, class U> class InsertRelation ;
		template <class T, class U> class InsertCollection ;
		template <class T, class U> class SqlInsertRelation ;
		template <class T, class U> class UpdateRelation ;
		template <class T, class U> class SqlUpdateRelation ;
		template <class T> class SqlSelect ;
		template <class T, class U> class SqlSelectCollection ;
		template <class T> class SqlUpdate ;
		class InitSchema ;
	}

	namespace mapping {
		class MappingInfo ;
		template <class T> class Mapping ;
		template <class T> class FieldRef ;
		template <class T> class PtrRef ;
		class FieldInfo ;
		template <class T> class PtrRef ;
		template <class T> class WeakRef ;
		template <class T> class CollectionRef ;
	}

	namespace stmt {
		class BulkStatement ;
		class PreparedStatement ;
	}

	namespace traits {
		template <class T> class dbo_traits ;
	}
//	template<class C> std::ostream& operator<<(std::ostream& o, const ptr<C>& _ptr) ;
}

#include <dbo/connection.h>
#include <dbo/ptr.hpp>
#include <dbo/weak_ptr.hpp>
#include <dbo/lazy_ptr.hpp>
#include <dbo/collection.hpp>
#include <dbo/relation.hpp>
#include <dbo/query.hpp>
#include <dbo/Exception.h>

#include <dbo/stmt/BulkStatement.h>
#include <dbo/stmt/PreparedStatement.h>

#include <dbo/traits/dbo_traits.hpp>
#include <dbo/traits/std_sql_traits.hpp>

#include <dbo/mapping/Mapping.hpp>
#include <dbo/mapping/CollectionRef.hpp>
#include <dbo/mapping/FieldRef.hpp>
#include <dbo/mapping/PtrRef.hpp>
#include <dbo/mapping/WeakRef.hpp>

#include <dbo/action/Debug.hpp>
#include <dbo/action/InitSchema.hpp>
#include <dbo/action/Delete.hpp>
#include <dbo/action/BulkInsert.hpp>
#include <dbo/action/Insert.hpp>
#include <dbo/action/InsertCollection.hpp>
#include <dbo/action/InsertRelation.hpp>
#include <dbo/action/Update.hpp>
#include <dbo/action/UpdateRelation.hpp>
#include <dbo/action/SqlUpdateRelation.hpp>
#include <dbo/action/SelectById.hpp>
#include <dbo/action/LoadDb.hpp>
#include <dbo/action/LoadId.hpp>
#include <dbo/action/SqlInsert.hpp>
#include <dbo/action/SqlInsertRelation.hpp>
#include <dbo/action/SqlSelect.hpp>
#include <dbo/action/SqlSelectCollection.hpp>
#include <dbo/action/SqlUpdate.hpp>

#include <dbo/traits/dbo_traits.hpp>

#endif
