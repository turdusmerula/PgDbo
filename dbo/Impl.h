// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_IMPL_H_
#define DBO_IMPL_H_

#include <dbo/Types.h>

#include <dbo/collection_impl.h>
#include <dbo/ptr_impl.h>
#include <dbo/weak_ptr_impl.h>
#include <dbo/Call_impl.h>
#include <dbo/DbAction_impl.h>
#include <dbo/Query_impl.h>
#include <dbo/Field_impl.h>
#include <dbo/SqlTraits_impl.h>
#include <dbo/Session_impl.h>

#if !defined(_MSC_VER) && !defined(__SUNPRO_C)
#define DBO_INSTANTIATE_TEMPLATES(C)					\
  template class dbo::ptr<C>;					\
  template class dbo::Dbo<C>;					\
  template class dbo::MetaDbo<C>;					\
  template class dbo::collection< dbo::ptr<C> >;		\
  template class dbo::Query< dbo::ptr<C>,			\
				 dbo::DynamicBinding >;		\
  template class dbo::Query< dbo::ptr<C>,			\
				 dbo::DirectBinding >;		\
  template dbo::ptr<C> dbo::Session::add<C>(ptr<C>&);		\
  template dbo::ptr<C> dbo::Session::add<C>(C *);		\
  template dbo::ptr<C> dbo::Session::load<C>			\
	(const dbo_traits<C>::IdType&, bool forceReread);		\
  template void dbo::Session::mapClass<C>(const char *);		\
  template struct dbo::Session::Mapping<C>;				\
  template dbo::Query< dbo::ptr<C>, dbo::DynamicBinding >	\
	dbo::Session::find<C, dbo::DynamicBinding>		\
	(const std::string&);						\
  template dbo::Query< dbo::ptr<C>, dbo::DirectBinding >	\
	dbo::Session::find<C, dbo::DirectBinding>		\
	(const std::string&);
#else
// Functionality is broken on MSVC 2005 and 2008
// Functionality is broken on Sun Studio Express
#define DBO_INSTANTIATE_TEMPLATES(C)
#endif

#endif // DBO_IMPL_H_
