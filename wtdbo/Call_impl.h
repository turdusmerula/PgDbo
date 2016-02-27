// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2010 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_CALL_IMPL_H_
#define DBO_CALL_IMPL_H_

namespace wtdbo
{

template<typename T> Call& Call::bind(const T& value)
{
	sql_value_traits<T>::bind(value, statement_, column_++, -1);

	return *this;
}

}

#endif // DBO_CALL_IMPL_H_
