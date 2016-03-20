namespace dbo {
namespace action {

template<class C>
LoadId<C>::LoadId(IdType& id, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	mapping_(mapping),
		stmt_(stmt),
		id_(id),
		state_(LoadIdState::PreparingStatement)
{
}

template<class C>
void LoadId<C>::visit()
{
	if(stmt_.prepared()==false)
		state_ = LoadIdState::PreparingStatement ;
	else
		state_ = LoadIdState::ReadingResult ;

	// read id
	id(*this, id_, mapping_->idName()) ;
}

template<class C>
template<typename V>
void LoadId<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case LoadIdState::PreparingStatement:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case LoadIdState::ReadingResult:
		if(traits::sql_value_traits<V>::read(field.value(), stmt_, -1)==false)
			throw Exception("Load error: read out of bounds") ;
		break ;
	}
}

template<class C>
template<typename V>
void LoadId<C>::actId(V& value, const std::string& name, int size)
{
	// add id fields to statement
	field(*this, value, name) ;
}

template<class C>
template<class D>
void LoadId<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	switch(state_)
	{
	case LoadIdState::PreparingStatement:
		break ;
	case LoadIdState::ReadingResult:
		field.value() = dbo::make_ptr<D>() ;
		break ;
	}

	// this action is C type, we need D, so we create a special one for this type
	LoadId<D> action(const_cast<IdType&>(field.value().id()), conn().template getMapping<D>(), stmt_) ;
	action.state_ = state_ ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
}

template<class C>
template <class D>
void LoadId<C>::actWeakPtr(const mapping::WeakRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	LoadId<D> action(const_cast<IdType&>(field.value().id()), conn().template getMapping<D>(), stmt_) ;
	action.state_ = state_ ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;
}

template<class C>
template<class D>
void LoadId<C>::actCollection(const mapping::CollectionRef<D>& field)
{

}

}}
