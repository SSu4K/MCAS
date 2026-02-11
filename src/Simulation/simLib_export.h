#ifndef SIMLIB_EXPORT_H
#define SIMLIB_EXPORT_H

#pragma once
#include <QtCore/qglobal.h>

#if defined(SIMLIB_LIBRARY)
#  define SIMLIB_API Q_DECL_EXPORT
#else
#  define SIMLIB_API Q_DECL_IMPORT
#endif

#endif // SIMLIB_EXPORT_H
