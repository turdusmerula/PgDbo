namespace dbo {
namespace action {

template<class C, class D>
SqlInsertRelation<C, D>::SqlInsertRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt)
	: 	field_(field),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt)
{
	data_ = std::make_shared<SqlInsertRelationData>() ;
}

template<class C, class D>
SqlInsertRelation<C, D>::SqlInsertRelation(const mapping::CollectionRef<C>& field, std::shared_ptr<mapping::Mapping<C>> mapping1, std::shared_ptr<mapping::Mapping<D>> mapping2, stmt::PreparedStatement& stmt, std::shared_ptr<SqlInsertRelationData> data)
	: 	field_(field),
		mapping1_(mapping1),
		mapping2_(mapping2),
		stmt_(stmt),
		data_(data)
{
}

template<class C, class D>
void SqlInsertRelation<C, D>::visit()
{
	std::stringstream& ss=data_->sql_ ;

	ss << "insert into \"" << field_.joinName() << "\" (" ;

	static typename traits::dbo_traits<D>::IdType dummy2 ;
	data_->tableName = mapping2_->tableName ;
	actId(dummy2, mapping2_->idName(), -1) ;

	static typename traits::dbo_traits<C>::IdType dummy1 ;
	data_->tableName = mapping1_->tableName ;
	actId(dummy1, mapping1_->idName(), -1) ;

	ss << ") values (" ;
	for(int i=0 ; i<data_->params_ ; i++)
	{
		if(i>0)
			ss << ", " ;
		ss << "?" ;
	}
	ss << ")" ;

	stmt_.sql(ss.str()) ;
}

template<class C, class D>
template<typename V>
void SqlInsertRelation<C, D>::act(const mapping::FieldRef<V>& field)
{
	std::stringstream& ss=data_->sql_ ;

	if(data_->params_>0)
		ss << ", " ;
	ss << "\"" << data_->tableName << "_" << field.name() << "\"" ;
	data_->params_++ ;
}

template<class C, class D>
template<typename V>
void SqlInsertRelation<C, D>::actId(V& value, const std::string& name, int size)
{
	field(*this, value, name) ;
}

template<class C, class D>
template<class E>
void SqlInsertRelation<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;

	// this action is C type, we need E, so we create a special one for this type
	SqlInsertRelation<C, E> action(field_, mapping1_, conn().template getMapping<E>(), stmt_, data_) ;
	id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping1_->idName()) ;
}

template<class C, class D>
template<class E>
void SqlInsertRelation<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
	using IdType=typename traits::dbo_traits<E>::IdType;

	// this action is C type, we need E, so we create a special one for this type
	SqlInsertRelation<C, E> action(field_, mapping1_, conn().template getMapping<E>(), stmt_, data_) ;
	id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping1_->idName()) ;
}

template<class C, class D>
template<class E>
void SqlInsertRelation<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
