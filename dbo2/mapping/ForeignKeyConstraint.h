#ifndef _DBO_MAPPING_FOREIGNKEYCONSTRAINT_H_
#define _DBO_MAPPING_FOREIGNKEYCONSTRAINT_H_

namespace dbo2 {

const int FKNone=0x00 ;
const int FKNotNull=0x01 ;
const int FKOnUpdateCascade=0x02 ;
const int FKOnUpdateSetNull=0x04 ;
const int FKOnDeleteCascade=0x08 ;
const int FKOnDeleteSetNull=0x10 ;

class ForeignKeyConstraint
{
public:
	explicit ForeignKeyConstraint(int value) ;

	int value() const {	return value_; }
private:
	int value_;
} ;


/*! \brief Combines two constraints.
 *
 * \ingroup dbo
 */
ForeignKeyConstraint operator|(ForeignKeyConstraint lhs, ForeignKeyConstraint rhs) ;

namespace fk {
/*! \brief A constraint that prevents a \c null ptr.
 *
 * A database constraint which prevents that a ptr references no object
 * and has a value of \c null.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint None(FKNone) ;

/*! \brief A constraint that prevents a \c null ptr.
 *
 * A database constraint which prevents that a ptr references no object
 * and has a value of \c null.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint NotNull(FKNotNull) ;

/*! \brief A constraint that cascades updates.
 *
 * A database constraint which propagates updates to the natural primary key
 * in the referenced table.
 *
 * \note This constraint only affects the database schema creation. Currently
 *       it is not possible to update a natural Id of an already saved object
 *       through %Dbo itself.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint OnUpdateCascade(FKOnUpdateCascade) ;

/*! \brief A constraint that cascades updates.
 *
 * A database constraint which sets the value of the ptr to null when the
 * referenced primary key changes.
 *
 * \note This constraint only affects the database schema creation. Currently
 *       it is not possible to update a natural Id of an already saved object
 *       through %Dbo itself.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint OnUpdateSetNull(FKOnUpdateSetNull) ;

/*! \brief A constraint that cascades deletes.
 *
 * A database constraint which propagates deletes of the referenced object
 * to also delete the object(s) that reference it.
 *
 * \note This constraint only affects the database schema creation.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint OnDeleteCascade(FKOnDeleteCascade) ;


/*! \brief A constraint that cascades deletes.
 *
 * A database constraint which propagates deletes of the referenced object
 * to also delete the objects that reference.
 *
 * \note This constraint only affects the database schema creation.
 *
 * \ingroup dbo
 */
const ForeignKeyConstraint OnDeleteSetNull(FKOnDeleteSetNull) ;

}

}

#endif
