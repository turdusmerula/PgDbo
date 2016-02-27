#ifndef _DBO_TRAITS_SQL_VALUE_TRAITS_HPP_
#define _DBO_TRAITS_SQL_VALUE_TRAITS_HPP_

namespace dbo {
namespace traits {

/*! \class sql_value_traits Dbo/SqlTraits Dbo/SqlTraits
 *  \brief Traits class for value types.
 *
 * This traits class may be specialized for a custom type \p V, to add
 * dbo support for custom types. A value type has a one-to-one mapping
 * to a single database column.
 *
 * The library has built-in support for:
 *  - <tt>std::string</tt>
 *  - <tt>char const *</tt> (read-only: only as a bound parameter for a query)
 *  - <tt>short</tt>, <tt>int</tt>, <tt>long long</tt>
 *  - <tt>long</tt>:  since the size of a <tt>long</tt> is 64bit on
 *    UNIX/Linux 64bit systems and 32bit otherwise, it is mapped to an
 *    <tt>integer</tt> or a <tt>bigint</tt> depending on the environment.
 *  - <tt>float</tt>, <tt>double</tt>
 *  - enum types
 *  - <tt>bool</tt>
 *  - <tt>std::vector<unsigned char></tt> (binary data)
 *  - <tt>boost::optional<T></tt>: to make the type optional
 *    (allowing an SQL <tt>null</tt> value)
 *  - <tt>boost::posix_time::ptime</tt>: time stamp, an invalid value (e.g.
 *    default constructed), maps to <tt>null</tt>
 *  - <tt>boost::posix_time::time_duration</tt>: time interval, an invalid
 *    value (boost::posix_time::not_a_date_time), maps to <tt>null</tt>
 *
 * \sa query_result_traits
 *
 * \ingroup dbo
 */
template<typename V, class Enable=void>
struct sql_value_traits
{
	static const bool not_specialized=true ;
} ;

}}

#endif
