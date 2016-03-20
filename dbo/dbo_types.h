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
		template <class T, class U> class SqlInsertRelation ;
		template <class T> class SqlSelectById ;
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

#endif