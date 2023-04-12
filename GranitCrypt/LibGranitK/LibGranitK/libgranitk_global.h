#pragma once

#include <QtCore/qglobal.h>

#ifndef QT_STATIC
# if defined(LIBGRANITK_LIB)
#  define LIBGRANITK_EXPORT Q_DECL_EXPORT
# else
#  define LIBGRANITK_EXPORT Q_DECL_IMPORT
# endif
#else
# define LIBGRANITK_EXPORT
#endif
