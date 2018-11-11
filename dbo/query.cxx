namespace dbo {

//template <class Result>
//query<Result>& query<Result>::select(const std::string& condition)
//{
//	sql_ += " where "+condition ;
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::where(const std::string& condition)
//{
//	sql_ += " where "+condition ;
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::orderBy(const std::string& fieldName)
//{
//	sql_ += " order by "+fieldname ;
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::groupBy(const std::string& fields)
//{
//	sql_ += " group by "+fields ;
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::having(const std::string& fields)
//{
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::offset(int count)
//{
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::offset(const std::string& count)
//{
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::limit(int count)
//{
//	return *this ;
//}
//
//template <class Result>
//query<Result>& query<Result>::limit(const std::string& count)
//{
//	return *this ;
//}

template <class C>
query& query::read(ptr<C>& ptr)
{
	if(hasrow_==false)
	{
		std::stringstream ss ;
		ss << "Query error: no data to fetch" ;
		ss << " -> " << stmt_.sql() ;
		ss << " -> (" << stmt_.getBoundPlaceholders() << ")" ;
		throw Exception(ss.str()) ;
	}

	auto mapping=conn_->getMapping<C>() ;

	if(ptr==nullptr)
		ptr = dbo::make_ptr<C>() ;
	action::LoadDb<C> action(ptr, mapping, stmt_) ;
	action.visit() ;

	return *this ;
}

template <class T>
query& query::read(T& value)
{
	if(hasrow_==false)
	{
		std::stringstream ss ;
		ss << "Query error: no data to fetch" ;
		ss << " -> " << stmt_.sql() ;
		ss << " -> (" << stmt_.getBoundPlaceholders() << ")" ;
		throw Exception(ss.str()) ;
	}

	if(traits::sql_value_traits<T>::read(value, stmt_, -1)==false)
		throw Exception("Load error: read out of bounds") ;

	return *this ;
}

template <class C>
query& query::bind(const C& value)
{
	traits::sql_value_traits<C>::bind(value, stmt_, -1) ;
	return *this ;
}



template <class C>
query::row& query::row::read(ptr<C>& ptr)
{
	if(query_==nullptr)
		throw Exception("Load error: read on empty row") ;

	query_->read<C>(ptr) ;

	return *this ;
}

template <class T>
query::row& query::row::read(T& value)
{
	if(query_==nullptr)
		throw Exception("Load error: read on empty row") ;

	query_->read<T>(value) ;

	return *this ;
}

}
