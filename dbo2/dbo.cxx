namespace dbo2 {

template<class Action, typename V>
void field(Action& action, V& value, const std::string& name, int size)
{
	action.act(mapping::FieldRef<V>(value, name, size)) ;
}

}
