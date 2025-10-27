#ifndef HEXINT_H
#define HEXINT_H

#include <QString>

namespace HexInt{
    QString intToString(const quint16 value, const bool prefix = true, const qsizetype precision = 4);
    quint16 hexStringToInt(const QString &string, bool* okptr);
    quint16 stringToInt(const QString &string, bool* okptr);
};

#endif // HEXINT_H
