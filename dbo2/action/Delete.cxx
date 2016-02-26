namespace dbo2 {
namespace action {

template<class C>
Delete<C>::Delete(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::PreparedStatement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		state_(PreparingStatement)
{
	if(ptr_.id()==traits::dbo_traits<C>::invalidId())
	{
		std::stringstream ss ;
		ss << "Delete failed for '" << mapping_->tableName << "': invalid id '" << ptr_.id() << "'" ;
		throw Exception(ss.str()) ;
	}
}

template<class C>
void Delete<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		state_ = PreparingStatement ;

		// init prepared statement, where clause is an id
		field(*this, const_cast<IdType&>(ptr_.id()), "") ;

		try {
			stmt_.prepare() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Delete statement preparation failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

	}

	state_ = Deleting ;
	if(ptr_)
	{
		stmt_.reset() ;

		// bind id to search for
		if(mapping_->surrogateIdFieldName!=boost::none)
			field(*this, const_cast<IdType&>(ptr_.id()), mapping_->surrogateIdFieldName.get()) ;
		else
			field(*this, const_cast<IdType&>(ptr_.id()), mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Select failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

		// all was right, set id
		ptr_.id(traits::dbo_traits<C>::invalidId()) ;
	}
}

template<class C>
template<typename V>
void Delete<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case PreparingStatement:
	case Deleting:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	}
}

template<class C>
template<typename V>
void Delete<C>::actId(V& value, const std::string& name, int size)
{
	// add id fields to statement
	field(*this, value, name) ;
}

template<class C>
template<class D>
void Delete<C>::actPtr(const mapping::PtrRef<D>& field)
{
	using IdType = typename traits::dbo_traits<D>::IdType ;

	// this action is C type, we need D, so we create a special one for this type
	Delete<D> action(field.value(), conn().template getMapping<D>(), stmt_) ;
	action.state_ = static_cast<typename Delete<D>::State>(state_) ;

	// load the id
	// objects are not loaded, only the id to be able to operate a lazy loading next
	id(action, const_cast<IdType&>(field.value().id()), field.name()) ;

}

}}
