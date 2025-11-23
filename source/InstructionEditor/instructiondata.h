#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include "Assembler/instruction.h"

namespace InstructionEditor {

const qsizetype MAX_LINES = 128;
const quint32 BASE_ADDRESS = 0;

struct InstructionEntry {
    quint32 encoded = 0;
    QString text = "NOP";
    std::shared_ptr<Assembly::Instruction> instruction;
    bool valid = true;
    QString errorMessage;
};

class InstructionData : public QObject
{
    Q_OBJECT
public:

    QList<InstructionEntry> instructions;
    const int maxLines;
    quint32 baseAddress;
    explicit InstructionData(QObject *parent = nullptr, int maxLines = MAX_LINES, quint32 baseAddress = BASE_ADDRESS);

signals:
};

} // namespace InstructionEditor

#endif // INSTRUCTIONDATA_H
