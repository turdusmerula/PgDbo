namespace dbo {
namespace action {

template<class C>
SqlInsert<C>::SqlInsert(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt)
{
	data_ = std::make_shared<SqlInsertData>() ;
}

template<class C>
SqlInsert<C>::SqlInsert(std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt, std::shared_ptr<SqlInsertData> data)
	:	stmt_(stmt),
		data_(data)
{
}

template<class C>
void SqlInsert<C>::visit()
{
	std::stringstream& ss=data_->sql_ ;

	ss << "insert into \"" << mapping_->tableName << "\" (" ;

	static C dummy ;
	dummy.persist(*this) ;

	ss << ") values (" ;
	for(int i=0 ; i<data_->params_ ; i++)
	{
		if(i>0)
			ss << ", " ;
		ss << "?" ;
	}
	ss << ")" ;

	if(mapping_->surrogateIdFieldName!=boost::none)
		ss << traits::SqlPostgresTypes::autoincrementInsertSuffix(mapping_->surrogateIdFieldName.get()) ;

	stmt_.sql(ss.str()) ;
}

template<class C>
template<typename V>
void SqlInsert<C>::act(const mapping::FieldRef<V>& field)
{
	std::stringstream& ss=data_->sql_ ;

	if(data_->params_>0)
		ss << ", " ;
	ss << "\"" << field.name() << "\"" ;
	data_->params_++ ;
}

template<class C>
template<typename V>
void SqlInsert<C>::actId(V& value, const std::string& name, int size)
{
	field(*this, value, name) ;
}

template<class C>
template<class D>
void SqlInsert<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;
	auto mapping=conn().template getMapping<D>() ;

	if(field.nameIsJoin()==false)
	{
		// not in a one to one connection

		// this action is C type, we need D, so we create a special one for this type
		SqlInsert<D> action(conn().template getMapping<D>(), stmt_, data_) ;
		id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping->idName()) ;
	}
}

template<class C>
template<class D>
void SqlInsert<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;
	auto mapping=conn().template getMapping<D>() ;

	if(field.nameIsJoin()==false)
	{
		// not in a one to one connection

		// this action is C type, we need D, so we create a special one for this type
		SqlInsert<D> action(conn().template getMapping<D>(), stmt_, data_) ;
		id(action, const_cast<IdType&>(field.value().id()), field.name()+"_"+mapping->idName()) ;
	}
}

template<class C>
template<class D>
void SqlInsert<C>::actCollection(const mapping::CollectionRef<D>& field)
{
}

}}
