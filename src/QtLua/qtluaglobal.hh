#ifndef QTLUAGLOBAL_HH_
#define QTLUAGLOBAL_HH_

#include <QtCore/qglobal.h>

#if defined(QTLUA_COMPILE_LIBRARY)
#  define QTLUA_LIB_DECL Q_DECL_EXPORT
#elif defined(QTLUA_USE_LIBRARY)
#  define QTLUA_LIB_DECL Q_DECL_IMPORT
#else
#  define QTLUA_LIB_DECL
#endif

#endif
