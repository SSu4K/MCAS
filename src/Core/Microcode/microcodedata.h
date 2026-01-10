#ifndef MICROCODEDATA_H
#define MICROCODEDATA_H

#include "instruction.h"
#include "microcodeconfig.h"
#include "Common/labeldata.h"

namespace Microcode{

    class MicrocodeData{
    public:
        MicrocodeConfig config;
        QList<Microcode::Instruction> instructions;
        LabelData labelData;

        MicrocodeData(const MicrocodeConfig &config);
        static Microcode::MicrocodeData buildMinimalFetchMicrocode(const MicrocodeConfig &config);
        QStringList getValidStringValues(const size_t field);
        bool isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs = Qt::CaseSensitive);
        QString matchValidFieldValue(const size_t field, const QString &string, bool * okptr);

        void eraseInstruction(const size_t row);
        void eraseAll();
        bool setValue(const size_t field, const size_t row, const QString &string);
    };

}
#endif // MICROCODEDATA_H
