#ifndef MODELLIB_EXPORT_H
#define MODELLIB_EXPORT_H

#pragma once
#include <QtCore/qglobal.h>

#if defined(MODELLIB_LIBRARY)
#  define MODELLIB_API Q_DECL_EXPORT
#else
#  define MODELLIB_API Q_DECL_IMPORT
#endif

#endif // MODELLIB_EXPORT_H
