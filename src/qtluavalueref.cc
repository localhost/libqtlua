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

#include <cassert>
#include <cstdlib>

#include <QtLua/ValueRef>

extern "C" {
#include <lua.h>
}

namespace QtLua {

  void ValueRef::copy_table_key(double tid, double kid)
  {
    if (!_st)
      return;

    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _table_id);
    lua_pushnumber(lst, tid);
    lua_rawget(lst, LUA_REGISTRYINDEX);  
    lua_rawset(lst, LUA_REGISTRYINDEX);

    lua_pushnumber(lst, _key_id);
    lua_pushnumber(lst, kid);
    lua_rawget(lst, LUA_REGISTRYINDEX);  
    lua_rawset(lst, LUA_REGISTRYINDEX);
  }

  void ValueRef::copy_table(double id)
  {
    if (!_st)
      return;

    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _table_id);
    lua_pushnumber(lst, id);
    lua_rawget(lst, LUA_REGISTRYINDEX);  
    lua_rawset(lst, LUA_REGISTRYINDEX);
  }

  void ValueRef::copy_key(double id)
  {
    if (!_st)
      return;

    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _key_id);
    lua_pushnumber(lst, id);
    lua_rawget(lst, LUA_REGISTRYINDEX);  
    lua_rawset(lst, LUA_REGISTRYINDEX);
  }

  void ValueRef::cleanup()
  {
    assert(_st);
    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _table_id);
    lua_pushnil(lst);
    lua_rawset(lst, LUA_REGISTRYINDEX);

    lua_pushnumber(lst, _key_id);
    lua_pushnil(lst);
    lua_rawset(lst, LUA_REGISTRYINDEX);
  }

  void ValueRef::push_value() const
  {
    check_state();
    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _table_id);
    lua_rawget(lst, LUA_REGISTRYINDEX);
    lua_pushnumber(lst, _key_id);
    lua_rawget(lst, LUA_REGISTRYINDEX);
    QTLUA_PROTECT_BEGIN(_st, p);
    lua_gettable(lst, -2);
    QTLUA_PROTECT_END(_st, p);
    lua_remove(lst, -2);
  }

  Value ValueRef::value() const
  {
    check_state();
    lua_State *lst = _st->_lst;

    {
      lua_pushnumber(lst, _table_id);
      lua_rawget(lst, LUA_REGISTRYINDEX);
      lua_pushnumber(lst, _key_id);
      lua_rawget(lst, LUA_REGISTRYINDEX);
      QTLUA_PROTECT_BEGIN(_st, p);
      lua_gettable(lst, -2);
      QTLUA_PROTECT_END(_st, p);
    }

    Value res(-1, _st);
    lua_pop(lst, 2);
    return res;
  }

  void ValueRef::table_set(const Value &v) const
  {
    check_state();
    lua_State *lst = _st->_lst;

    lua_pushnumber(lst, _table_id);
    lua_rawget(lst, LUA_REGISTRYINDEX);

    int t = lua_type(lst, -1);

    switch (t)
      {
      case Value::TUserData: {
	UserData::ptr ud = UserData::pop_ud(lst);

	if (!ud.valid())
	  throw String("Can not index null lua::userdata value.");

	Value k(_st);
	k._id = _key_id;	// reuse stored key id for temp Value

	try {
	  ud->meta_newindex(_st, k, v);
	} catch (...) {
	  k._st = 0;
	  throw;
	}
	k._st = 0;

	return;
      }

      case Value::TTable:
	lua_pushnumber(lst, _key_id);
	lua_rawget(lst, LUA_REGISTRYINDEX);
	if (lua_isnil(lst, -1))
	  {
	    lua_pop(lst, 2);
	  }
	else
	  {
	    v.push_value();
	    QTLUA_PROTECT_BEGIN(_st, p);
	    lua_settable(lst, -3);
	    QTLUA_PROTECT_END(_st, p);
	    lua_pop(lst, 1);
	  }
	return;

      default:
	lua_pop(lst, 1);
	throw String("Can not index lua::% value.").arg(lua_typename(lst, t));
      }
  }

}

