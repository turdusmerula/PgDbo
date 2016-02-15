namespace dbo2 {

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



template<class A, class C>
void belongsToImpl(A& action, ptr<C>& value, const std::string& name, int fkConstraints, int size)
{
	if(name.empty())
		action.actPtr(mapping::PtrRef<C>(value, action.conn().template tableName<C>(), size, fkConstraints));
	else
		action.actPtr(mapping::PtrRef<C>(value, name, size, fkConstraints));
}

template<class A, class C>
void belongsTo(A& action, ptr<C>& value, const std::string& name, int size)
{
	belongsToImpl(action, value, name, 0, size);
}

}
