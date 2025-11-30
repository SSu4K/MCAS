#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include "Assembly/instruction.h"

namespace InstructionEditor {

struct InstructionEntry {
    quint32 encoded = 0;
    QString label = "";
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
    explicit InstructionData(QObject *parent = nullptr, int maxLines = 0, quint32 baseAddress = 0);

signals:
};

} // namespace InstructionEditor

#endif // INSTRUCTIONDATA_H
