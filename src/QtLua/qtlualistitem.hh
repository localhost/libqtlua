/*
    This file is part of LibQtLua.

    LibQtLua is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    LibQtLua is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with LibQtLua.  If not, see <http://www.gnu.org/licenses/>.

    Copyright (C) 2008, Alexandre Becoulet <alexandre.becoulet@free.fr>

*/


#ifndef QTLUALISTITEM_HH_
#define QTLUALISTITEM_HH_

#include <QHash>
#include <QList>

#include "qtluaitem.hh"
#include "qtluaiterator.hh"

namespace QtLua {

class ItemModel;

  /**
   * @short Qt Model/View list item class
   * @header QtLua/ListItem
   * @module {Model/View}
   *
   * This class together with the @ref Item and @ref ItemModel classes
   * enable easy use of list or hierarchical data structures that can be
   * viewed and modified from lua script, Qt view widgets and C++
   * code.
   *
   * @ref ListItem objects are @ref Item objects with pointer list to
   * children objects. It can be accessed as tables from lua script.
   *
   * See @ref ItemModel for example.
   */

class ListItem : public Item
{
  friend class Item;
  friend class ItemModel;

public:

  QTLUA_REFTYPE(ListItem);

  ListItem();
  ~ListItem();

  /** Find a child item from name. */
  inline Ref<Item> get_child(const String &name) const;

  /** Get child items list */
  inline const QList<Ref<Item> > & get_list() const;

  /** Get number of childs */
  inline int get_child_count() const;

  Value meta_operation(State *ls, Value::Operation op, const Value &a, const Value &b);
  bool support(Value::Operation c) const;
  void meta_newindex(State *ls, const Value &key, const Value &value);
  Value meta_index(State *ls, const Value &key);
  bool meta_contains(State *ls, const Value &key);
  Ref<Iterator> new_iterator(State *ls);

protected:

  /**
   * This function can be reimplemented to allow or deny items
   * membership when inserted from lua script or Qt view.
   *
   * @return true if item is allowed to be a child member.
   */
  virtual bool accept_child(const Ref<Item> &item) const;

  /** Must return columns count for children of this node, default implementation returns 1. */
  virtual int get_column_count() const;

  /** This function is called when a child is added, removed or
      changes name. Default implementation does nothing. */
  virtual void child_changed();

  /** This function returns default name of the child item used when
      first inserted if not already set. */
  virtual String default_child_name(int row) const;

private:

  void completion_patch(String &path, String &entry, int &offset);

  void set_model(ItemModel* model);

  void change_indexes(int first);
  void insert(Item *item, int row);
  void insert_name(Item *item, int row);
  void remove(Item *item);
  inline void remove_name(Item *item);

  QHash<String,Item*> _child_hash;
  QList<Ref<Item> > _child_list;
  int _id_counter;
};

}

#endif

