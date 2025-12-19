#ifndef MICROCODECONFIG_H
#define MICROCODECONFIG_H

namespace Microcode {

class MicrocodeConfig
{
public:
    MicrocodeConfig();
    QMap<size_t, QString> fieldNames;
    QMap<size_t, QStringList> validValues;
    size_t microcodeSize = 512;

    QStringList getValidValues(size_t field) const;
    QString getFieldName(size_t field) const;
};

} // namespace Microcode

#endif // MICROCODECONFIG_H
