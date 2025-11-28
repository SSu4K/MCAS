#include "twoscomplement.h"

qint32 TwoC::getModulus(const qsizetype bitWidth) {
    return (1u << bitWidth);
}

qint32 TwoC::getMin(const qsizetype bitWidth) {
    return -(1 << (bitWidth - 1));
}

qint32 TwoC::getMax(const qsizetype bitWidth) {
    return (1 << (bitWidth - 1)) - 1;
}

qint32 TwoC::toInt(const quint32 value, const qsizetype bitWidth) {
    const quint32 modulus = getModulus(bitWidth);
    const quint32 signBit = (1u << (bitWidth - 1));
    quint32 result = value;

    result &= (modulus - 1);

    // Check sign bit
    if (result & signBit)
        return static_cast<qint32>(result) - static_cast<qint32>(modulus);

    return static_cast<qint32>(result);
}

quint32 TwoC::toComplement(const qint32 value, const qsizetype bitWidth) {
    const qint32 min = getMin(bitWidth);
    const qint32 max = getMax(bitWidth);
    const quint32 modulus = getModulus(bitWidth);
    quint32 result = value;

    if (value < min) result = min;
    if (value > max) result = max;

    if (result >= 0)
        return static_cast<quint32>(result);

    return static_cast<quint32>(modulus + result);


}
