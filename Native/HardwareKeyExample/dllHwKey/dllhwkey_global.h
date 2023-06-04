#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(DLLHWKEY_LIB)
#  define DLLHWKEY_EXPORT Q_DECL_EXPORT
# else
#  define DLLHWKEY_EXPORT Q_DECL_IMPORT
# endif
#else
# define DLLHWKEY_EXPORT
#endif
