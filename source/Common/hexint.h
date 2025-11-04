#ifndef HEXINT_H
#define HEXINT_H

#include <QString>

namespace HexInt{
    QString intToString(const quint32 value, const bool prefix = true, const qsizetype precision = 4);
    quint32 hexStringToInt(const QString &string, bool* okptr);
    quint32 stringToInt(const QString &string, bool* okptr);
};

#endif // HEXINT_H
