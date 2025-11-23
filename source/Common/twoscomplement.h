#ifndef TWOSCOMPLEMENT_H
#define TWOSCOMPLEMENT_H

namespace TwoC{
qint32 getModulus(const qsizetype bitWidth);
qint32 getMin(const qsizetype bitWidth);
qint32 getMax(const qsizetype bitWidth);

qint32 toInt(const quint32 value, const qsizetype bitWidth);
quint32 toComplement(const qint32 value, const qsizetype bitWidth);

}

#endif // TWOSCOMPLEMENT_H
