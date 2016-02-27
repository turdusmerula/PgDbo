namespace dbo {

template<class A, typename V>
void id(A& action, V& value, const std::string& name, int size)
{
	action.actId(value, name, size) ;
}

template<class Action, typename V>
void field(Action& action, V& value, const std::string& name, int size)
{
	action.act(mapping::FieldRef<V>(value, name, size)) ;
}


template<class Action, class C>
void belongsTo(Action& action, ptr<C>& value, const std::string& name, ForeignKeyConstraint constraints, int size)
{
	if(name.empty())
		action.actPtr(mapping::PtrRef<C>(value, action.conn().template tableName<C>(), size, constraints.value())) ;
	else
		action.actPtr(mapping::PtrRef<C>(value, name, size, constraints.value())) ;
}

template<class A, class C>
void belongsTo(A& action, ptr<C>& value, const std::string& name, int size)
{
	belongsTo(action, value, name, fk::None, size) ;
}

template<class Action, class C>
void belongsTo(Action& action, ptr<C>& value, ForeignKeyConstraint constraints, int size)
{
	belongsTo(action, value, "", fk::None, size) ;

}

template<class Action, class C>
void hasMany(Action& action, collection<C>& value, RelationType type, const std::string& joinName)
{
	action.actCollection(mapping::CollectionRef<C>(value, type, joinName, std::string(), FKNotNull|FKOnDeleteCascade)) ;
}

template<class Action, class C>
void hasMany(Action& action, collection<C>& value, RelationType type, const std::string& joinName, const std::string& joinId, ForeignKeyConstraint constraint)
{
	if(type!=ManyToMany)
		throw Exception("hasMany() with named joinId only for a ManyToMany relation");

	action.actCollection(mapping::CollectionRef<C>(value, type, joinName, joinId, constraint.value())) ;
}

}
