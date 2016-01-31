// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_TYPES_H_
#define DBO_TYPES_H_

#include <dbo/ptr.h>
#include <dbo/weak_ptr.h>
#include <dbo/collection.h>
#include <dbo/Call.h>
#include <dbo/DbAction.h>
#include <dbo/Exception.h>
#include <dbo/Field.h>
#include <dbo/Query.h>
#include <dbo/Session.h>
#include <dbo/StdSqlTraits.h>
#include <dbo/PostgisSqlTraits.h>
#include <dbo/ptr_tuple.h>

#if !defined(_MSC_VER) && !defined(__SUNPRO_C)
#define DBO_EXTERN_TEMPLATES(C)						\
  extern template class dbo::ptr<C>;				\
  extern template class dbo::Dbo<C>;				\
  extern template class dbo::MetaDbo<C>;				\
  extern template class dbo::collection< dbo::ptr<C> >;		\
  extern template class dbo::Query< dbo::ptr<C>,		\
					dbo::DynamicBinding >;	\
  extern template class dbo::Query< dbo::ptr<C>,		\
					dbo::DirectBinding >;	\
  extern template dbo::ptr<C> dbo::Session::add<C>(ptr<C>&);	\
  extern template dbo::ptr<C> dbo::Session::add<C>(C *);	\
  extern template dbo::ptr<C> dbo::Session::load<C>		\
	(const dbo_traits<C>::IdType&, bool forceReread);		\
  extern template void dbo::Session::mapClass<C>(const char *);	\
  extern template struct dbo::Session::Mapping<C>;			\
  extern template dbo::Query< dbo::ptr<C>,			\
				  dbo::DynamicBinding>		\
	dbo::Session::find<C, dbo::DynamicBinding>		\
	(const std::string&);						\
  extern template dbo::Query< dbo::ptr<C>,			\
				  dbo::DirectBinding>		\
	dbo::Session::find<C, dbo::DirectBinding>		\
	(const std::string&);

#else
// Broken on MSVC 2005 and 2008 (cannot redeclare extern declared template
// methods to be defined after all)
// Broken on SUN StudioExpress
#define DBO_EXTERN_TEMPLATES(C)
#include <Dbo/Impl>
#endif

#endif // DBO_TYPES_H_
