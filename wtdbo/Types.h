// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_TYPES_H_
#define DBO_TYPES_H_

#include <wtdbo/ptr.h>
#include <wtdbo/weak_ptr.h>
#include <wtdbo/collection.h>
#include <wtdbo/Call.h>
#include <wtdbo/DbAction.h>
#include <wtdbo/Exception.h>
#include <wtdbo/Field.h>
#include <wtdbo/Query.h>
#include <wtdbo/Session.h>
#include <wtdbo/StdSqlTraits.h>
#include <wtdbo/PostgisSqlTraits.h>
#include <wtdbo/ptr_tuple.h>

#if !defined(_MSC_VER) && !defined(__SUNPRO_C)
#define DBO_EXTERN_TEMPLATES(C)						\
  extern template class wtdbo::ptr<C>;				\
  extern template class wtdbo::Dbo<C>;				\
  extern template class wtdbo::MetaDbo<C>;				\
  extern template class wtdbo::collection< wtdbo::ptr<C> >;		\
  extern template class wtdbo::Query< wtdbo::ptr<C>,		\
					wtdbo::DynamicBinding >;	\
  extern template class wtdbo::Query< wtdbo::ptr<C>,		\
					wtdbo::DirectBinding >;	\
  extern template wtdbo::ptr<C> wtdbo::Session::add<C>(ptr<C>&);	\
  extern template wtdbo::ptr<C> wtdbo::Session::add<C>(C *);	\
  extern template wtdbo::ptr<C> wtdbo::Session::load<C>		\
	(const dbo_traits<C>::IdType&, bool forceReread);		\
  extern template void wtdbo::Session::mapClass<C>(const char *);	\
  extern template struct wtdbo::Session::Mapping<C>;			\
  extern template wtdbo::Query< wtdbo::ptr<C>,			\
				  wtdbo::DynamicBinding>		\
	wtdbo::Session::find<C, wtdbo::DynamicBinding>		\
	(const std::string&);						\
  extern template wtdbo::Query< wtdbo::ptr<C>,			\
				  wtdbo::DirectBinding>		\
	wtdbo::Session::find<C, wtdbo::DirectBinding>		\
	(const std::string&);

#else
// Broken on MSVC 2005 and 2008 (cannot redeclare extern declared template
// methods to be defined after all)
// Broken on SUN StudioExpress
#define DBO_EXTERN_TEMPLATES(C)
#include <Dbo/Impl>
#endif

#endif // DBO_TYPES_H_
