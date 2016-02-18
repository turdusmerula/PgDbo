namespace dbo2 {
namespace action {

template<class C>
Insert<C>::Insert(ptr<C> ptr, std::shared_ptr<mapping::Mapping<C>> mapping, stmt::Statement& stmt)
	: 	ptr_(ptr),
		mapping_(mapping),
		stmt_(stmt),
		state_(PreparingStatement),
		id_(traits::dbo_traits<C>::invalidId())
{
	if(!ptr)
	{
		std::stringstream ss ;
		ss << "Insert error for '" << mapping_->tableName << "': passing null ptr" ;
		throw Exception(ss.str()) ;
	}

}

template<class C>
void Insert<C>::visit()
{
	auto ptr=const_cast<C*>(ptr_.get()) ;

	if(stmt_.prepared()==false)
	{
		// init prepared statement, use a dummy object to init the statement
		state_ = PreparingStatement ;
		C dummy ;
		dummy.persist(*this) ;

		stmt_.prepare() ;
	}

	if(ptr_)
	{
		state_ = Inserting ;
		stmt_.reset() ;
		ptr->persist(*this) ;

		try {
			stmt_.execute() ;
		} catch(std::exception& e) {
			std::stringstream ss ;
			ss << "Insert failed for '" << mapping_->tableName << "': " << e.what() ;
			throw Exception(ss.str()) ;
		}

		state_ = ReadingId ;
		if(stmt_.hasReturning())
		{
			// inserted ok, insert result should now contain the generated id
			if(stmt_.nextRow()==false)
			{
				std::stringstream ss ;
				ss << "Insert error for '" << mapping_->tableName << "': no returning id" ;
				throw Exception(ss.str()) ;
			}

			// read id
			if(mapping_->surrogateIdFieldName!=boost::none)
				field(*this, const_cast<IdType&>(ptr_.id()), mapping_->surrogateIdFieldName.get()) ;
			else
				field(*this, const_cast<IdType&>(ptr_.id()), mapping_->naturalIdFieldName, mapping_->naturalIdFieldSize) ;
		}
		else
		{
			// inserted ok, set the cached id
			ptr_.id(id_) ;
		}
	}
}

template<class C>
template<typename V>
void Insert<C>::act(const mapping::FieldRef<V>& field)
{
	switch(state_)
	{
	case PreparingStatement:
	case Inserting:
		traits::sql_value_traits<V>::bind(field.value(), stmt_, -1) ;
		break ;
	case ReadingId:
		traits::sql_value_traits<V>::read(field.value(), stmt_, -1) ;
		break ;
	}
}

template<class C>
template<typename V>
void Insert<C>::actId(V& value, const std::string& name, int size)
{
	switch(state_)
	{
	case PreparingStatement:
		// add id fields to statement
		field(*this, value, name) ;
		break ;
	case Inserting:
		if(value==traits::dbo_traits<C>::invalidId())
		{
			std::stringstream ss ;
			ss << "Insert failed for '" << mapping_->tableName << "' invalid id '" << ptr_.id() << "'" ;
			throw Exception(ss.str()) ;
		}

		// in case of a statement with a natural id we see the id here, put it in cache
		id_ = value ;

		// add id fields to statement
		field(*this, value, name) ;
		break ;
	case ReadingId:

		break ;
	}
}

}}
