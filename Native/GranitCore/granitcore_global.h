#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(GRANITCORE_LIB)
#  define GRANITCORE_EXPORT Q_DECL_EXPORT
# else
#  define GRANITCORE_EXPORT Q_DECL_IMPORT
# endif
#else
# define GRANITCORE_EXPORT
#endif
