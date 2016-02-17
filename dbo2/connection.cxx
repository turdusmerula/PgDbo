
namespace dbo2 {

template<class C>
void connection::mapClass(std::string tableName)
{
	if(schemaInitialized_)
		throw Exception("Cannot map tables after schema was initialized.") ;

	if(classRegistry_.find(&typeid(C))!=classRegistry_.end())
		throw Exception("Class "+std::string(typeid(C).name())+" already mapped") ;
	if(tableRegistry_.find(tableName)!=tableRegistry_.end())
		throw Exception("Table "+tableName+" already mapped") ;

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

	action::Insert<C> action(obj, mapping, stmt) ;
	action.visit() ;

	return obj ;
}

template<class C>
ptr<C> connection::update(ptr<C>& obj)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlUpdate)->second ;

	action::Update<C> action(obj, mapping, stmt) ;
	action.visit() ;

	return obj ;
}

template<class C>
ptr<C> connection::load(ptr<C>& obj)
{
//	auto mapping=getMapping<C>() ;
//	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlSelectById)->second ;
//
//	action::SaveDb<C> action(obj, mapping, stmt) ;
//	action.visit() ;
//
//	return obj ;
}

template<class C>
ptr<C> connection::load(const typename traits::dbo_traits<C>::IdType& id)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlSelectById)->second ;

	ptr<C> obj=make_ptr<C>() ;

	action::LoadDb<C> action(obj, id, mapping, stmt) ;
	action.visit() ;

	return obj ;
}

}