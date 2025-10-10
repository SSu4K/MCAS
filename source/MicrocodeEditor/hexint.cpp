#include "hexint.h"

QString HexInt::intToString(const quint16 value, const bool prefix, const qsizetype precision){
    quint16 twosComplement = static_cast<quint16>(value);
    if(prefix){
        return QString("0x") + QString("%1").arg(value, precision, 16, QLatin1Char('0')).toUpper();
    }
    else{
        return QString("%1").arg(value, precision, 16, QLatin1Char('0')).toUpper();
    }
}

quint16 HexInt::hexStringToInt(const QString &string, bool* okptr)
{
    quint16 value = 0;
    QString s = string.trimmed();

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        value = s.mid(2).toUShort(okptr, 16);
    } else {
        value = s.toUShort(okptr, 16);
    }

    return *okptr ? value : 0;
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
