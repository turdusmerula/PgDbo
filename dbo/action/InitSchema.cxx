#include <typeinfo>

namespace dbo {
namespace action {

template<class C>
void InitSchema::visit(C& obj)
{
	mapping_.surrogateIdFieldName = traits::dbo_traits<C>::surrogateIdField() ;

	obj.persist(*this) ;
}

template<typename V>
void InitSchema::act(const mapping::FieldRef<V>& field)
{
	int flags=mapping::FieldInfo::Mutable | mapping::FieldInfo::NeedsQuotes ;

	if(idField_)
		flags |= mapping::FieldInfo::NaturalId ; // Natural id

	if(!foreignKeyName_.empty())
		// Foreign key
		mapping_.fields.push_back(mapping::FieldInfo(field.name(), &typeid(V), field.sqlType(), foreignKeyTable_, foreignKeyName_, flags|mapping::FieldInfo::ForeignKey, fkConstraints_)) ;
	else
		// Normal field
		mapping_.fields.push_back(mapping::FieldInfo(field.name(), &typeid(V), field.sqlType(), flags)) ;
}

template<typename V>
void InitSchema::actId(V& value, const std::string& name, int size)
{
	mapping_.naturalIdFieldName = name ;
	mapping_.naturalIdFieldSize = size ;

	if(mapping_.surrogateIdFieldName)
		throw Exception("Error: dbo::id() called for class C "
				"with surrogate key: "
				"dbo::dbo_traits<C>::surrogateIdField() != 0") ;

	idField_ = true ;
	field(*this, value, name, size) ;
	idField_ = false ;
}

template<class C>
void InitSchema::actPtr(const mapping::PtrRef<C>& field)
{
	if(field.nameIsJoin()==true)
	{
		std::string joinTableName=conn_.tableName<C>() ;
		std::string joinName = field.name() ;
		if(joinName.empty())
			joinName = mapping_.tableName;

		mapping_.sets.push_back(mapping::SetInfo(joinTableName, OneToOne, joinName, std::string(), 0)) ;
	}
	else
	{
		std::shared_ptr<mapping::Mapping<C>> mapping=conn_.getMapping<C>() ;

		bool setName = foreignKeyName_.empty() ;

		if(setName)
		{
			foreignKeyName_ = field.name() ;
			foreignKeyTable_ = mapping->tableName ;
			fkConstraints_ = field.fkConstraints() ;
		}

		field.visit(*this, conn_);

		if(setName)
		{
			foreignKeyName_.clear();
			foreignKeyTable_.clear();
			fkConstraints_ = 0 ;
		}
	}
}

template<class C>
void InitSchema::actWeakPtr(const mapping::WeakRef<C>& field)
{
	if(field.nameIsJoin()==true)
	{
		std::string joinTableName=conn_.tableName<C>() ;
		std::string joinName = field.name() ;
		if(joinName.empty())
			joinName = mapping_.tableName;

		mapping_.sets.push_back(mapping::SetInfo(joinTableName, OneToOne, joinName, std::string(), 0)) ;
	}
	else
	{
		std::shared_ptr<mapping::Mapping<C>> mapping=conn_.getMapping<C>() ;

		bool setName = foreignKeyName_.empty() ;

		if(setName)
		{
			foreignKeyName_ = field.name() ;
			foreignKeyTable_ = mapping->tableName ;
			fkConstraints_ = field.fkConstraints() ;
		}

		field.visit(*this, conn_);

		if(setName)
		{
			foreignKeyName_.clear();
			foreignKeyTable_.clear();
			fkConstraints_ = 0 ;
		}
	}
}


template<class C>
void InitSchema::actCollection(const mapping::CollectionRef<C>& field)
{
	std::string joinTableName=conn_.tableName<C>() ;
	std::string joinName=field.joinName() ;

	if(joinName.empty())
		joinName = createJoinName(field.type(), mapping_.tableName, joinTableName) ;

	mapping_.sets.push_back(mapping::SetInfo(joinTableName, field.type(), joinName, field.joinId(), field.fkConstraints())) ;
}

}}
