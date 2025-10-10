#include "hexint.h"

QString HexInt::intToString(const quint16 value){
    quint16 twosComplement = static_cast<quint16>(value);
    return QString("0x") + QString("%1").arg(value, 4, 16, QLatin1Char('0')).toUpper();
}

quint16 HexInt::stringToInt(const QString &string, bool* okptr)
{
    *okptr = false;
    quint16 value = 0;

    QString s = string.trimmed();

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        value = s.mid(2).toUShort(okptr, 16);
    } else {
        value = s.toUShort(okptr, 10);
    }

    return *okptr ? value : 0;
}
