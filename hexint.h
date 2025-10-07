#ifndef HEXINT_H
#define HEXINT_H

#include <QString>

namespace HexInt{
    QString intToString(const quint16 value);
    quint16 stringToInt(const QString &string, bool* okptr);
};

#endif // HEXINT_H
