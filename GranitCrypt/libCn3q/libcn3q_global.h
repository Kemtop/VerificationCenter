#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(LIBCN3Q_LIB)
#  define LIBCN3Q_EXPORT Q_DECL_EXPORT
# else
#  define LIBCN3Q_EXPORT Q_DECL_IMPORT
# endif
#else
# define LIBCN3Q_EXPORT
#endif
