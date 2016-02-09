template<class C>
void dbo2::database::mapClass(const char* tableName)
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
