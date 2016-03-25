namespace dbo {
namespace action {


template<class C, class D>
InsertCollection<C,  D>::InsertCollection(ptr<C>& ptr, collection<D>& coll, std::shared_ptr<mapping::Mapping<C>> mappingC, std::shared_ptr<mapping::Mapping<D>> mappingD, stmt::PreparedStatement& stmt)
	:	mappingC_(mappingC),
		mappingD_(mappingD),
		stmt_(stmt),
		ptr_(ptr),
		coll_(coll),
		state_(PreparingStatement)
{
	data_ = std::make_shared<InsertCollectionData>() ;
}

template<class C, class D>
void InsertCollection<C, D>::visit()
{
	ptr_->persist(*this) ;

	if(data_->collExist_==false)
	{
		std::stringstream ss ;
		ss << "Insert failed: The specified collection to table " << mappingD_->tableName << " from table " << mappingC_->tableName <<  " at address " << &coll_ << " does not belong to object " << ptr_.get()  ;
		throw Exception(ss.str()) ;
	}
}

template<class C, class D>
template<typename E>
void InsertCollection<C, D>::act(const mapping::FieldRef<E>& field)
{
}

template<class C, class D>
template<typename E>
void InsertCollection<C, D>::actId(E& value, const std::string& name, int size)
{
}

template<class C, class D>
template<class E>
void InsertCollection<C, D>::actPtr(const mapping::PtrRef<E>& field)
{
}

template<class C, class D>
template<class E>
void InsertCollection<C, D>::actWeakPtr(const mapping::WeakRef<E>& field)
{
}

template<class C, class D>
template<class E, typename std::enable_if<std::is_same<E, D>::value, int>::type>
void InsertCollection<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
	// test if we are dealing with the same field
	if(&coll_!=&field.value())
		return ;

	if(field.type()==ManyToMany)
	{
		auto mappingE=conn().template getMapping<E>() ;

		for(auto ptr : field.value())
		{
			// insert into join table
			InsertRelation<E, C> action(ptr, ptr_, field, mappingE, mappingC_, stmt_) ;
			action.visit() ;
		}
	}
	else
	{
		std::stringstream ss ;
		ss << "Insert failed for '" << mappingC_->tableName << "': not yet implemented" ;
		throw Exception(ss.str()) ;

	}

	data_->collExist_ = true ;
}

template<class C, class D>
template<class E, typename std::enable_if<!std::is_same<E, D>::value, int>::type>
void InsertCollection<C, D>::actCollection(const mapping::CollectionRef<E>& field)
{
}

}}
