#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(PRODUCTKEYLIB_LIB)
#  define PRODUCTKEYLIB_EXPORT Q_DECL_EXPORT
# else
#  define PRODUCTKEYLIB_EXPORT Q_DECL_IMPORT
# endif
#else
# define PRODUCTKEYLIB_EXPORT
#endif
