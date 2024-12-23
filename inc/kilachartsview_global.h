#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(KILACHARTSVIEW_LIB)
#  define KILACHARTSVIEW_EXPORT Q_DECL_EXPORT
# else
#  define KILACHARTSVIEW_EXPORT Q_DECL_IMPORT
# endif
#else
# define KILACHARTSVIEW_EXPORT
#endif
