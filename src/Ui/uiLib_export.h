#ifndef UILIB_EXPORT_H
#define UILIB_EXPORT_H

#pragma once
#include <QtCore/qglobal.h>

#if defined(UILIB_LIBRARY)
#  define UILIB_API Q_DECL_EXPORT
#else
#  define UILIB_API Q_DECL_IMPORT
#endif

#endif // UILIB_EXPORT_H
