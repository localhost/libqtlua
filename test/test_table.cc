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

#include "test.hh"

#include <QtLua/State>
#include <QtLua/Value>

using namespace QtLua;

int main()
{
  try {

  {
    QtLua::State ls;

    ls["t"] = Value::new_table(&ls);
    ls["t"]["a"] = 5;
    ls.at("t")["b"] = ls.at("t").at("a");
    ls["t"]["c"] = ls["t"]["b"].value();

    ASSERT(ls.at("t").at("c").to_integer() == 5);
    ASSERT(ls["t"]["c"].to_integer() == 5);
  }

  {
    int j;
    QtLua::State ls;

    ls.openlib(QtLuaLib);

    ls.exec_statements("t={a=1, b=\"foo\", c=3}");

    ASSERT(ls.at("t")["a"].value().to_integer() == 1);
    ASSERT(ls["t"]["b"].value().to_string() == "foo");
    ASSERT(ls.at("t").at("c").to_integer() == 3);

    QtLua::Value t = ls.at("t");

    j = 0;
    for (QtLua::Value::iterator i = t.begin(); i != t.end(); i++, j++)
      ASSERT(t.at(i.key()) == i.value().value());
    ASSERT(j == 3);

    j = 0;
    for (QtLua::Value::const_iterator i = t.begin(); i != t.end(); i++, j++)
      ASSERT(t.at(i.key()) == i.value());
    ASSERT(j == 3);

    for (QtLua::Value::iterator i = t.begin(); i != t.end(); i++, j++)
      *i = String(i.key().to_string() + "_foo");

    ASSERT(ls.at("t").at("a").to_string() == "a_foo");
    ASSERT(ls.at("t").at("b").to_string() == "b_foo");
    ASSERT(ls.at("t").at("c").to_string() == "c_foo");

    ASSERT(ls.exec_statements("i=0; r={}; for key, value in each(t) do r[key]=value..\"bar\"; i=i+1 end; return i").at(0).to_integer() == 3);

    ASSERT(ls.at("r").at("a").to_string() == "a_foobar");
    ASSERT(ls.at("r").at("b").to_string() == "b_foobar");
    ASSERT(ls.at("r").at("c").to_string() == "c_foobar");
  }

  } catch (QtLua::String &e) {
    std::cout << e.constData() << std::endl;
    ASSERT(0);
  }

  return 0;
}

