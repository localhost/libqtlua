
/* anchor 1 */
#include <QtLua/Function>
#include <QtLua/Plugin>

#include "plugin.hh"

Q_EXPORT_PLUGIN2(example, ExamplePlugin);

QTLUA_FUNCTION(foo, "The foo function", "No help available")
{
  Q_UNUSED(args);
  return QtLua::Value(ls, "foo");
}

void ExamplePlugin::register_members(QtLua::Plugin &plugin)
{
  QTLUA_PLUGIN_FUNCTION_REGISTER(plugin, foo);
}
/* anchor end */

struct LoadUnload
{
  LoadUnload()
  {
    qDebug("example plugin loaded");
  }

  ~LoadUnload()
  {
    qDebug("example plugin unloaded");
  }
} load_unload;

