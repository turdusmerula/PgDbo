#ifndef _DBO_TRAITS_DBO_DEFAULT_TRAITS_HPP_
#define _DBO_TRAITS_DBO_DEFAULT_TRAITS_HPP_

/*! \class dbo_default_traits Dbo/Dbo Dbo/Dbo
 *  \brief Default traits for a class mapped with %%Dbo.
 *
 * This class provides the default traits. It is convenient (and
 * future proof) to inherit these default traits when customizing the
 * traits for one particular class.
 *
 * \ingroup dbo
 */
struct dbo_default_traits
{
	/*! \brief Type of the primary key.
	 *
	 * This indicates the type of the primary key, which needs to be
	 * <tt>long long</tt> for a surrogate id, but can be any type
	 * supported by dbo::field() (including composite types) for a
	 * natural primary key.
	 *
	 * The following operations need to be supported for an id value:
	 *
	 *  - <i>default constructor</i>
	 *  - <i>copy constructor</i>
	 *  - serialization to a string (for formatting an error message in exceptions)
	 *    : <tt>std::ostream << id</tt>
	 *  - comparison operator (for use as a key in a std::map): <tt>id == id</tt>
	 *  - less than operator (for use as a key in a std::map): <tt>id < id</tt>
	 *
	 * Only the default <tt>long long</tt> is supported for an
	 * auto-incrementing surrogate primary key. You need to change the
	 * default key type typically in conjuction with specifying a natural id,
	 * see dbo::id().
	 *
	 * The following example illustrates how to prepare a type to be
	 * usable as a composite id type:
	 *
	 * \code
	 * struct Coordinate {
	 *   int x, y;
	 *
	 *   Coordinate()
	 *     : x(-1), y(-1) { }
	 *
	 *   bool operator== (const Coordinate& other) const {
	 *     return x == other.x && y == other.y;
	 *   }
	 *
	 *   bool operator< (const Coordinate& other) const {
	 *     if (x < other.x)
	 *       return true;
	 *     else if (x == other.x)
	 *       return y < other.y;
	 *     else
	 *       return false;
	 *   }
	 * };
	 *
	 * std::ostream& operator<< (std::ostream& o, const Coordinate& c)
	 * {
	 *   return o << "(" << c.x << ", " << c.y << ")";
	 * }
	 *
	 *   namespace dbo {
	 *
	 *     template <class Action>
	 *     void field(Action& action, Coordinate& coordinate, const std::string& name, int size = -1)
	 *     {
	 *       field(action, coordinate.x, name + "_x");
	 *       field(action, coordinate.y, name + "_y");
	 *     }
	 *   }
	 * \endcode
	 */
	typedef long long IdType ;

	/*! \brief Returns the sentinel value for a \c null id.
	 *
	 * When used as a foreign key, this value is used to represent a \c
	 * null value.
	 *
	 * The default implementation returns -1.
	 */
	static IdType invalidId()
	{
		return -1 ;
	}

	/*! \brief Configures the surrogate primary key field.
	 *
	 * Returns the field name which is the surrogate primary key,
	 * corresponding to the object's id.
	 *
	 * You can disable this auto-incrementing surrogate id by returning
	 * \c 0 instead. In that case you will need to define a natural id
	 * for your class using dbo::id().
	 *
	 * The default surrogate id database field name is <tt>"id"</tt>.
	 */
	static boost::optional<std::string> surrogateIdField()
	{
		return std::string("id") ;
	}
} ;

#endif
