#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(LIBHWKEY_LIB)
#  define LIBHWKEY_EXPORT Q_DECL_EXPORT
# else
#  define LIBHWKEY_EXPORT Q_DECL_IMPORT
# endif
#else
# define LIBHWKEY_EXPORT
#endif
