#ifndef MICROCODEDATA_H
#define MICROCODEDATA_H

#include "instruction.h"
#include "microcodeconfig.h"

namespace Microcode{

    class MicrocodeData{
    public:
        MicrocodeConfig config;
        QList<Microcode::Instruction> instructions;

        MicrocodeData(const MicrocodeConfig &config);
        QStringList getValidStringValues(const size_t field);
        bool isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs = Qt::CaseSensitive);
        QString matchValidFieldValue(const size_t field, const QString &string, bool * okptr);

    };

}
#endif // MICROCODEDATA_H
