// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_IMPL_H_
#define DBO_IMPL_H_

#include <wtdbo/Types.h>

#include <wtdbo/collection_impl.h>
#include <wtdbo/ptr_impl.h>
#include <wtdbo/weak_ptr_impl.h>
#include <wtdbo/Call_impl.h>
#include <wtdbo/DbAction_impl.h>
#include <wtdbo/Query_impl.h>
#include <wtdbo/Field_impl.h>
#include <wtdbo/SqlTraits_impl.h>
#include <wtdbo/Session_impl.h>

#if !defined(_MSC_VER) && !defined(__SUNPRO_C)
#define DBO_INSTANTIATE_TEMPLATES(C)					\
  template class wtdbo::ptr<C>;					\
  template class wtdbo::Dbo<C>;					\
  template class wtdbo::MetaDbo<C>;					\
  template class wtdbo::collection< wtdbo::ptr<C> >;		\
  template class wtdbo::Query< wtdbo::ptr<C>,			\
				 wtdbo::DynamicBinding >;		\
  template class wtdbo::Query< wtdbo::ptr<C>,			\
				 wtdbo::DirectBinding >;		\
  template wtdbo::ptr<C> wtdbo::Session::add<C>(ptr<C>&);		\
  template wtdbo::ptr<C> wtdbo::Session::add<C>(C *);		\
  template wtdbo::ptr<C> wtdbo::Session::load<C>			\
	(const dbo_traits<C>::IdType&, bool forceReread);		\
  template void wtdbo::Session::mapClass<C>(const char *);		\
  template struct wtdbo::Session::Mapping<C>;				\
  template wtdbo::Query< wtdbo::ptr<C>, wtdbo::DynamicBinding >	\
	wtdbo::Session::find<C, wtdbo::DynamicBinding>		\
	(const std::string&);						\
  template wtdbo::Query< wtdbo::ptr<C>, wtdbo::DirectBinding >	\
	wtdbo::Session::find<C, wtdbo::DirectBinding>		\
	(const std::string&);
#else
// Functionality is broken on MSVC 2005 and 2008
// Functionality is broken on Sun Studio Express
#define DBO_INSTANTIATE_TEMPLATES(C)
#endif

#endif // DBO_IMPL_H_
