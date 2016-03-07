namespace dbo {
namespace action {

template<class C, class D>
InsertRelation<C, D>::InsertRelation(ptr<C>& ptr1, ptr<D>& ptr2,
		const mapping::CollectionRef<C>& field,
		std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2,
		stmt::PreparedStatement& stmt)
	: 	field_(field),
		ptr1_(ptr1),
		ptr2_(ptr2),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt)
{
}

template<class C, class D>
void InsertRelation<C, D>::visit()
{
	if(stmt_.prepared()==false)
	{
		stmt_.reset() ;

		action::SqlInsertRelation<C, D> action(field_, mapping1_, mapping2_, stmt_) ;
		action.visit() ;

        // init prepared statement, use a dummy object to init the statement
		static typename traits::dbo_traits<D>::IdType dummy2 ;
		actId(dummy2, mapping2_->idName(), -1) ;

		static typename traits::dbo_traits<C>::IdType dummy1 ;
		actId(dummy1, mapping1_->idName(), -1) ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Insert statement preparation failed for relation '" << field_.joinName() << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}
	}

	stmt_.reset() ;

	if(ptr2_.id()==traits::dbo_traits<D>::invalidId())
	{
		std::stringstream ss ;
		ss << "Insert failed for relation '" << field_.joinName() << "' from '" << mapping2_->tableName << "' , invalid id " << mapping2_->idName() << "='" << ptr2_.id() << "'" ;
		throw Exception(ss.str()) ;
	}
	actId(const_cast<typename traits::dbo_traits<D>::IdType&>(ptr2_.id()), mapping2_->idName(), -1) ;

	if(ptr1_.id()==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Insert failed for relation '" << field_.joinName() << "' from '" << mapping1_->tableName << "' , invalid id " << mapping1_->idName() << "='" << ptr1_.id() << "'" ;
		throw Exception(ss.str()) ;
	}
	actId(const_cast<typename traits::dbo_traits<C>::IdType&>(ptr1_.id()), mapping1_->idName(), -1) ;

	try {
		stmt_.execute() ;
	} catch(std::exception& e) {
		std::stringstream ss ;
		ss << "Insert failed for relation '" << field_.joinName() << "': " << e.what() ;
		throw Exception(ss.str()) ;
	}

}

template<class C, class D>
template<typename V>
void InsertRelation<C, D>::act(const mapping::FieldRef<V>& field)
{
	traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
}

template<class C, class D>
template<typename V>
void InsertRelation<C, D>::actId(V& value, const std::string& name, int size)
{
	field(*this, value, name) ;
}

template<class C, class D>
template<class E>
void InsertRelation<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;

	// this action is C type, we need E, so we create a special one for this type
	InsertRelation<C, E> action(ptr1_, field.value(), field_, mapping1_, conn().template getMapping<E>(), stmt_) ;
	id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping1_->idName()) ;
}

template<class C, class D>
template<class E>
void InsertRelation<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;

	// this action is C type, we need E, so we create a special one for this type
	InsertRelation<C, E> action(ptr1_, field.value(), field_, mapping1_, conn().template getMapping<E>(), stmt_) ;
	id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping1_->idName()) ;
}

template<class C, class D>
template<class E>
void InsertRelation<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
