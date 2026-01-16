#include "hexint.h"

QString HexInt::intToString(const quint32 value, const bool prefix, const qsizetype precision){
    quint32 twosComplement = static_cast<quint32>(value);
    if(prefix){
        return QString("0x") + QString("%1").arg(value, precision, 16, QLatin1Char('0')).toUpper();
    }
    else{
        return QString("%1").arg(value, precision, 16, QLatin1Char('0')).toUpper();
    }
}

quint32 HexInt::hexStringToInt(const QString &string, bool* okptr)
{
    quint32 value = 0;
    QString s = string.trimmed();

    bool ok;

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        value = s.mid(2).toULong(&ok, 16);
    } else {
        value = s.toULong(&ok, 16);
    }

    if(okptr != nullptr) *okptr = ok;

    return ok ? value : 0;
}

quint32 HexInt::stringToInt(const QString &string, bool* okptr)
{
    *okptr = false;
    quint32 value = 0;

    QString s = string.trimmed();

    if (s.startsWith("0x", Qt::CaseInsensitive)) {
        value = s.mid(2).toUShort(okptr, 16);
    } else {
        value = s.toUShort(okptr, 10);
    }

    return *okptr ? value : 0;
}
