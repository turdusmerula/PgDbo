
namespace dbo2 {

template<class C>
void connection::mapClass(std::string tableName)
{
	if(schemaInitialized_)
		throw Exception("Cannot map tables after schema was initialized.") ;

	if(classRegistry_.find(&typeid(C))!=classRegistry_.end())
		return ;

	std::shared_ptr<mapping::Mapping<C>> mapping=std::make_shared<mapping::Mapping<C>>() ;
	mapping->tableName = tableName ;

	classRegistry_[&typeid(C)] = mapping ;
	tableRegistry_[tableName] = mapping ;
}

template<class C>
const std::string& connection::tableName() const
{
	typedef typename boost::remove_const<C>::type MutC ;

	ClassRegistry::const_iterator ireg=classRegistry_.find(&typeid(MutC)) ;
	if(ireg!=classRegistry_.end())
		return std::dynamic_pointer_cast<mapping::Mapping<MutC>>(ireg->second)->tableName ;
	else
		throw Exception(std::string("Class ")+typeid(MutC).name()+" was not mapped.") ;
}

template<class C>
std::shared_ptr<mapping::Mapping<C>> connection::getMapping()
{
	ClassRegistry::const_iterator ireg=classRegistry_.find(&typeid(C)) ;
	if(ireg!=classRegistry_.end())
	{
		std::shared_ptr<mapping::Mapping<C>> mapping=std::dynamic_pointer_cast<mapping::Mapping<C>>(ireg->second) ;
		return mapping ;
	}
	else
		throw Exception(std::string("Class ")+typeid(C).name()+" was not mapped.") ;
}

template<class C>
ptr<C> connection::insert(ptr<C>& obj)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlInsert)->second ;

	if(stmt.prepared()==false)
	{
		// init prepared statement
		action::InitStatement<C> action(mapping, stmt) ;
		action.visit() ;
	}

	action::SaveDb<C> action(obj, mapping, stmt , 0) ;
	action.visit() ;

	return obj ;
}

}
