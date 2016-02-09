#ifndef _DBO_TRAITS_DBO_TRAITS_HPP_
#define _DBO_TRAITS_DBO_TRAITS_HPP_

#include <dbo2/traits/dbo_default_traits.hpp>

namespace dbo2 {
namespace traits {

/*! \class dbo_traits Dbo/Dbo Dbo/Dbo
 *  \brief Traits for a class mapped with %%Dbo.
 *
 * The traits class provides some of the mapping properties related to
 * the primary key and optimistic concurrency locking using a version
 * field.
 *
 * See dbo_default_traits for default values.
 *
 * The following example changes the surrogate id field name for a
 * class <tt>Foo</tt> from the default <tt>"id"</tt> to
 * <tt>"foo_id"</tt>:
 *
 * \code
 *   namespace dbo {
 *
 *     template<>
 *     struct dbo_traits<Foo> : dbo_default_traits
 *     {
 *        static const char *surrogateIdField() { return "foo_id"; }
 *     };
 *
 *     // Necessary if you want to use ptr<const Foo>
 *     template<> struct dbo_traits<const Foo> : dbo_traits<Foo> {};
 *   }
 * \endcode
 *
 * \note The safe pattern to define traits is before the class definition,
 *       based on a forward declaration.
 *       This is necessary since the persist() function relies on
 *       this specialization:
 * \code
 * class Foo;
 *
 *   namespace dbo {
 *     template<> struct dbo_traits<Foo> : ... { };
 *   }
 *
 * class Foo {
 *   // definition here, including the persist() function
 * };
 * \endcode
 * \ingroup dbo
 */
template<class C>
struct dbo_traits : public dbo_default_traits
{
} ;

}}

#endif
