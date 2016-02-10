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
void belongsToImpl(A& action, key<C>& value, const std::string& name, int fkConstraints, int size)
{
//	if(name.empty())
//		action.actPtr(PtrRef<C>(value, Session::current()->template tableName<C>(), size, fkConstraints));
//	else
		action.actKey(mapping::KeyRef<C>(value, name, size, fkConstraints));
}

template<class A, class C>
void belongsTo(A& action, key<C>& value, const std::string& name, int size)
{
	belongsToImpl(action, value, name, 0, size);
}

}
