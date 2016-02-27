#ifndef _DBO_RELATION_HPP_
#define _DBO_RELATION_HPP_

#include <string>

#include <dbo/mapping/ForeignKeyConstraint.h>
#include <dbo/mapping/SetInfo.h>

namespace dbo {

/*! \brief Maps a natural primary key (id) field.
 *
 * A natural primary key field is optional. If you define one and its
 * type is not <tt>long long</tt>, you must specialize
 * dbo::dbo_traits to match the type \p V as the IdType for this
 * class. When not specified for a class, an auto-generated surrogate
 * key field is used with the name specified by
 * dbo::dbo_traits::surrogateIdField(), which defaults to "id".
 *
 * Unlike the default surrogate key, a natural id is not
 * auto-generated and thus you need to give each object a unique value
 * when creating a new object.
 *
 * The id may be a composite type. In that case, you need to
 * specialize dbo::field().
 *
 * \ingroup dbo
 */
template<class Action, typename V>
void id(Action& action, V& value, const std::string& name="id", int size=-1) ;


/*! \brief Maps a database object field.
 *
 * This function binds the field \p value to the database field \p name.
 *
 * The optional \p size may be used as a hint for the needed
 * storage. It is only useful for <i>std::string</i> or
 * <i>WString</i> fields, and causes the schema to use a
 * <tt>varchar(</tt><i><tt>size</tt></i><tt>)</tt> for storing the
 * field instead of an unlimited length string type.
 *
 * You may want to specialize this method for a particular composite
 * type which should be persisted in multiple database fields but not as
 * a separate table (e.g. for natural composite primary keys, see id()).
 *
 * For example:
 * \code
 * struct Coordinate {
 *   int x, y;
 * };
 *
 *   namespace dbo {
 *
 *     template <class Action>
 *     void field(Action& action, Coordinate& coordinate, const std::string& name, int size = -1)
 *     {
 *       field(action, coordinate.x, name + "_x");
 *       field(action, coordinate.y, name + "_y");
 *     }
 *
 *   } // namespace dbo
 * \endcode
 *
 * To support a custom type that needs to be persisted as a single
 * field, you should specialize sql_value_traits instead.
 *
 * \ingroup dbo
 */
template<class Action, typename V>
void field(Action& action, V& value, const std::string& name, int size=-1) ;


/*! \brief Maps the "One"-side (foreign key) of a ManyToOne or OneToOne relation.
 *
 * This function binds the pointer field \p value to the database
 * foreign key field(s) \p name <tt>+ "_" +</tt> (C's primary key(s)).
 *
 * If the name is omitted or empty, then C's mapped table name is used.
 *
 * A belongsTo() will usually have a counter-part hasMany() or
 * hasOne() declaration in the referenced class \p C.
 *
 * \sa hasMany()
 *
 * \ingroup dbo
 */
template<class Action, class C>
void belongsTo(Action& action, ptr<C>& value, const std::string& name=std::string(), int size=-1) ;

template<class Action, class C>
void belongsTo(Action& action, ptr<C>& value, const std::string& name, ForeignKeyConstraint constraints, int size=-1) ;

template<class Action, class C>
void belongsTo(Action& action, ptr<C>& value, ForeignKeyConstraint constraints, int size=-1) ;


template<class Action, class C>
void hasMany(Action& action, collection<C>& value, RelationType type, const std::string& name=std::string()) ;

template<class Action, class C>
void hasMany(Action& action, collection<C>& value, RelationType type, const std::string& name, const std::string& joinId, ForeignKeyConstraint constraints=(fk::NotNull|fk::OnDeleteCascade)) ;

}



#endif
