
namespace dbo2 {

template<class C>
void database::mapClass(std::string tableName)
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
const std::string& database::tableName() const
{
	typedef typename boost::remove_const<C>::type MutC ;

	ClassRegistry::const_iterator ireg=classRegistry_.find(&typeid(MutC)) ;
	if(ireg!=classRegistry_.end())
		return std::dynamic_pointer_cast<mapping::Mapping<MutC>>(ireg->second)->tableName ;
	else
		throw Exception(std::string("Class ")+typeid(MutC).name()+" was not mapped.") ;
}

template<class C>
std::shared_ptr<mapping::Mapping<C>> database::getMapping()
{
//	if(!schemaInitialized_)
//		initSchema() ;

	ClassRegistry::const_iterator ireg=classRegistry_.find(&typeid(C)) ;
	if(ireg!=classRegistry_.end())
	{
		std::shared_ptr<mapping::Mapping<C>> mapping=std::dynamic_pointer_cast<mapping::Mapping<C>>(ireg->second) ;
		return mapping ;
	}
	else
		throw Exception(std::string("Class ")+typeid(C).name()+" was not mapped.") ;
}

}
