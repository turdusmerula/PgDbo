
namespace dbo {

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

template <class C, class Statement>
Statement& connection::getStatement(mapping::MappingInfo::StatementType type)
{
	const_typeinfo_ptr id=&typeid(C) ;
	std::shared_ptr<Statement> res ;

	auto istmts=statements_.find(id) ;
	if(istmts==statements_.end())
		statements_[id] = {} ;

	if(statements_[id].size()<mapping::MappingInfo::StatementTypeCount)
	{
		for(int i=0 ; i<mapping::MappingInfo::StatementTypeCount ; i++)
			statements_[id].push_back(std::shared_ptr<stmt::Statement>()) ;
	}

	if(statements_[id][type]==nullptr)
		statements_[id][type] = std::make_shared<Statement>(*this) ;
	res = std::dynamic_pointer_cast<Statement>(statements_[id][type]) ;

	BOOST_ASSERT( statements_[id][type]==res ) ;
	return *(res.get()) ;
}

template<class C>
ptr<C>& connection::insert(ptr<C>& obj, ActionOption opt)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=getStatement<C, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;
	//auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlInsert)->second ;

	obj.tableName(tableName<C>().c_str()) ;

	action::Insert<C> action(obj, mapping, stmt, opt) ;
	action.visit() ;

	return obj ;
}

template<class C>
ptr<C> connection::insert(weak_ptr<C>& obj, ActionOption opt)
{
	ptr<C> ptr(obj) ;
	auto mapping=getMapping<C>() ;
	auto& stmt=getStatement<C, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlInsert) ;
//	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlInsert)->second ;

	ptr.tableName(tableName<C>().c_str()) ;

	action::Insert<C> action(ptr, mapping, stmt, opt) ;
	action.visit() ;

	return obj ;
}

template<class C>
collection<C>& connection::bulk_insert(collection<C>& coll)
{
	auto& mapping=*getMapping<C>() ;
	coll.tableName(tableName<C>().c_str()) ;

	action::BulkInsert<C> action(coll, mapping, *this) ;
	action.visit() ;

	return coll ;
}

template<class C>
ptr<C> connection::update(ptr<C>& obj, ActionOption opt)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=getStatement<C, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlUpdate) ;
//	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlUpdate)->second ;

	action::Update<C> action(obj, mapping, stmt, opt) ;
	action.visit() ;

	return obj ;
}

template<class C>
ptr<C> connection::load(ptr<C>& obj)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=getStatement<C, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlSelectById) ;
//	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlSelectById)->second ;

	action::SelectById<C> action(obj, mapping, stmt) ;
	action.visit() ;

	return obj ;
}

template<class C>
ptr<C> connection::load(const typename traits::dbo_traits<C>::IdType& id)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=getStatement<C, stmt::PreparedStatement>(mapping::MappingInfo::StatementType::SqlSelectById) ;
//	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlSelectById)->second ;

	ptr<C> obj=make_ptr<C>() ;
	obj.tableName(tableName<C>().c_str()) ;

	action::SelectById<C> action(obj, id, mapping, stmt) ;
	action.visit() ;

	return obj ;
}

template<class C>
void connection::remove(ptr<C>& obj)
{
	auto mapping=getMapping<C>() ;
	auto& stmt=mapping->statements.find(mapping::MappingInfo::SqlDelete)->second ;

	action::Delete<C> action(obj, mapping, stmt) ;
	action.visit() ;
//
//	return obj ;
}

template<class C>
query connection::find(const std::string& condition)
{
	if(condition.empty())
		return dbo::query(*this, "select * from \""+tableName<C>()+"\"") ;
	else
		return dbo::query(*this, "select * from \""+tableName<C>()+"\" where "+condition) ;
}

template<class C>
query connection::find(const collection<C>& collection, const std::string& condition)
{

}

}
