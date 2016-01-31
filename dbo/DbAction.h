// This may look like C code, but it's really -*- C++ -*-
/*
 * Copyright (C) 2008 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */
#ifndef DBO_DBACTION_H_
#define DBO_DBACTION_H_

#include <set>
#include <string>
#include <sstream>
#include <vector>

#include <dbo/ptr.h>
#include <dbo/collection.h>
#include <dbo/Field.h>
#include <dbo/SqlStatement.h>
#include <dbo/Session.h>

#include <boost/utility/enable_if.hpp>
#include <boost/type_traits/is_base_of.hpp>

  namespace dbo {

template <class C, class Enable = void>
struct persist
{
  template <class A> static void apply(C& obj, A& action);
};

template <class A>
struct action_sets_value : boost::false_type { };

class SqlStatement;

class  InitSchema
{
public:
  InitSchema(Session& session, Impl::MappingInfo& mapping);

  template<class C> void visit(C& obj);

  void actMapping(Impl::MappingInfo *mapping);
  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

  Session *session() { return &session_; }

private:
  Session& session_;
  Impl::MappingInfo& mapping_;

  bool idField_;
  std::string foreignKeyTable_, foreignKeyName_;
  int fkConstraints_;
};

class  DropSchema
{
public:
  DropSchema(Session& session, 
	     Impl::MappingInfo& mapping,
	     std::set<std::string>& tablesCreated);

  template<class C> void visit(C& obj);

  void actMapping(Impl::MappingInfo *mapping);
  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

  Session *session() { return &session_; }

private:
  Session& session_;
  Impl::MappingInfo& mapping_;
  std::set<std::string>& tablesDropped_;

  void drop(const std::string& tableName);
};

class  DboAction
{
public:
  DboAction(Session *session);
  DboAction(MetaDboBase& dbo, Impl::MappingInfo& mapping);

  void actMapping(Impl::MappingInfo *mapping);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

protected:
  MetaDboBase& dbo() { return *dbo_; }
  Impl::MappingInfo& mapping() { return *mapping_; }
  int setStatementIdx() const { return setStatementIdx_; }
  
private:
  Session *session_;
  MetaDboBase *dbo_;
  Impl::MappingInfo *mapping_;

  int setStatementIdx_, setIdx_;
};

class  LoadBaseAction : public DboAction
{
public:
  LoadBaseAction(MetaDboBase& dbo, Impl::MappingInfo& mapping,
		 SqlStatement *statement, int& column);

  template<typename V> void act(const FieldRef<V>& field);
  template<class D> void actPtr(const PtrRef<D>& field);

  bool setsValue() const;

protected:
  SqlStatement *statement_;
  int& column_;

  void start();
};

template <>
struct action_sets_value<LoadBaseAction> : boost::true_type { };

template <class C>
class LoadDbAction : public LoadBaseAction
{
public:
  LoadDbAction(MetaDbo<C>& dbo, Session::Mapping<C>& mapping,
	       SqlStatement *statement, int& column);

  void visit(C& obj);

  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class D> void actId(ptr<D>& value, const std::string& name, int size,
			       int fkConstraints);

private:
  MetaDbo<C>& dbo_;
};

template <class C>
struct action_sets_value<LoadDbAction<C> > : boost::true_type { };

class  SaveBaseAction : public DboAction
{
public:
  /*
   * Provide a statement and column if you want to abuse this action
   * to simply bind values to a statement using field().
   */
  SaveBaseAction(Session *session, SqlStatement *statement, int column);

  SaveBaseAction(MetaDboBase& dbo, Impl::MappingInfo& mapping,
		 SqlStatement *statement = 0, int column = 0);

  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;

  int column() const { return column_; }

protected:
  SqlStatement *statement_;
  bool isInsert_;
  int column_;
  bool bindNull_;

  enum { Dependencies, Self, Sets } pass_;
  bool needSetsPass_;

  void startDependencyPass();
  void startSelfPass();
  void startSetsPass();

  void exec();
};

template <class C>
class SaveDbAction : public SaveBaseAction
{
public:
  SaveDbAction(MetaDbo<C>& dbo, Session::Mapping<C>& mapping);

  void visit(C& obj);

  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class D> void actId(ptr<D>& value, const std::string& name, int size,
			       int fkConstraints);

private:
  MetaDbo<C>& dbo_;
};

class  TransactionDoneAction : public DboAction
{
public:
  TransactionDoneAction(MetaDboBase& dbo, Session& session,
			Impl::MappingInfo& mapping, bool success);

  template<class C> void visit(C& obj);

  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;

  Session *session() { return &session_; }

private:
  Session& session_;
  bool success_;
};

class  SessionAddAction : public DboAction
{
public:
  SessionAddAction(MetaDboBase& dbo, Impl::MappingInfo& mapping);

  template<class C> void visit(C& obj);

  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;
};

class  SetReciproceAction
{
public:
  SetReciproceAction(Session *session,
		     const std::string& joinName, MetaDboBase *value);

  template<class C> void visit(C& obj);

  void actMapping(Impl::MappingInfo *mapping);
  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

  Session *session() { return session_; }

private:
  Session *session_;
  const std::string& joinName_;
  MetaDboBase *value_;
};

template <>
struct action_sets_value<SetReciproceAction> : boost::true_type { };

class  ToAnysAction
{
public:
  ToAnysAction(std::vector<boost::any>& result);

  template<class C> void visit(const ptr<C>& obj);

  void actMapping(Impl::MappingInfo *mapping);
  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

  Session *session() { return session_; }

private:
  Session *session_;
  std::vector<boost::any>& result_;
  bool allEmpty_;
};

class  FromAnyAction
{
public:
  FromAnyAction(int& index, const boost::any& value);

  template<class C> void visit(const ptr<C>& obj);

  void actMapping(Impl::MappingInfo *mapping);
  template<typename V> void actId(V& value, const std::string& name, int size);
  template<class C> void actId(ptr<C>& value, const std::string& name, int size,
			       int fkConstraints);
  template<typename V> void act(const FieldRef<V>& field);
  template<class C> void actPtr(const PtrRef<C>& field);
  template<class C> void actWeakPtr(const WeakPtrRef<C>& field);
  template<class C> void actCollection(const CollectionRef<C>& field);

  bool getsValue() const;
  bool setsValue() const;
  bool isSchema() const;

  Session *session() { return session_; }

private:
  Session *session_;
  int& index_;
  const boost::any& value_;
};

template <>
struct action_sets_value<FromAnyAction> : boost::true_type { };

template<typename V>
void SaveBaseAction::act(const FieldRef<V>& field)
{
  if (pass_ == Self) {
    if (bindNull_)
      statement_->bindNull(column_++);
    else
      field.bindValue(statement_, column_++);
  }
}

  }

#endif // DBO_DBACTION_H_
