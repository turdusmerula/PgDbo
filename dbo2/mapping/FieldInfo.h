#ifndef _DBO_MAPPING_FIELDINFO_H_
#define _DBO_MAPPING_FIELDINFO_H_

#include <string>

namespace dbo2 {
namespace mapping {

/*! \class FieldInfo Dbo/SqlTraits Dbo/SqlTraits
 *  \brief Description of a field.
 *
 * \sa query_result_traits::getFields(), Query::fields()
 *
 * \ingroup dbo
 */
class FieldInfo
{
public:
	/*! \brief Flags
	 */
	enum Flags
	{
		SurrogateId = 0x1, //!< Field is a surrogate id
		NaturalId = 0x2,   //!< Field is (part of) a natural id
		Version = 0x4,     //!< Field is an optimistic concurrency version field
		Mutable = 0x8,     //!< Field can be edited
		NeedsQuotes = 0x10,     //!< Field name needs quotes when using in SQL
		ForeignKey = 0x20, //!< Field is (part of) a foreign key
		FirstDboField = 0x40
	};

	/*! \brief Creates a field description.
	 */
	FieldInfo(const std::string& name, const std::type_info *type, const std::string& sqlType, int flags) ;

	/*! \brief Creates a field description.
	 */
	FieldInfo(const std::string& name, const std::type_info *type, const std::string& sqlType, const std::string& foreignKeyTable, const std::string& foreignKeyName, int flags, int fkConstraints);

	/*! \brief Sets a qualifier for the field.
	 */
	void setQualifier(const std::string& qualifier, bool firstQualified = false);

	/*! \brief Returns the field name.
	 */
	const std::string& name() const
	{
		return name_;
	}

	/*! \brief Returns the field SQL type.
	 */
	const std::string& sqlType() const
	{
		return sqlType_;
	}

	/*! \brief Returns the field qualifier.
	 */
	const std::string& qualifier() const
	{
		return qualifier_;
	}

	/*! \brief Returns the field type.
	 */
	const std::type_info *type() const
	{
		return type_;
	}

	/*! \brief Returns whether the field is an Id field.
	 */
	bool isIdField() const
	{
		return (flags_&(SurrogateId|NaturalId))!=0;
	}

	/*! \brief Returns whether the field is a Natural Id field.
	 */
	bool isNaturalIdField() const
	{
		return (flags_&NaturalId)!=0;
	}

	/*! \brief Returns whether the field is a Surroaget Id field.
	 */
	bool isSurrogateIdField() const
	{
		return flags_&SurrogateId;
	}

	/*! \brief Returns whether the field is a Version field.
	 */
	bool isVersionField() const
	{
		return (flags_&Version)!=0;
	}

	/*! \brief Returns whether the field is mutable.
	 */
	bool isMutable() const
	{
		return (flags_&Mutable)!=0;
	}

	/*! \brief Returns whether the field name needs to be quoted.
	 */
	bool needsQuotes() const
	{
		return (flags_&NeedsQuotes)!=0;
	}

	/*! \brief Returns whether the field is part of a foreign key.
	 */
	bool isForeignKey() const
	{
		return (flags_&ForeignKey)!=0;
	}

	bool isFirstDboField() const
	{
		return (flags_&FirstDboField)!=0;
	}
	std::string foreignKeyName() const
	{
		return foreignKeyName_;
	}
	std::string foreignKeyTable() const
	{
		return foreignKeyTable_;
	}
	int fkConstraints() const
	{
		return fkConstraints_;
	}

	std::string sql() const;

private:
	std::string name_, sqlType_, qualifier_;
	std::string foreignKeyName_, foreignKeyTable_;
	const std::type_info *type_;
	int flags_;
	int fkConstraints_;
};

#endif

}}
