#ifndef INSTRUCTIONDATA_H
#define INSTRUCTIONDATA_H

#include <QObject>
#include <QHash>
#include "instruction.h"

namespace InstructionEditor {

const qsizetype MAX_LINES = 128;
const quint32 BASE_ADDRESS = 0;

// instructions accepted, constant for now, make dynamic later
inline InstructionSet DEFAULT_INSTRUCTION_SET = {
    {
        { "NOP",    {0, InstructionType::R, ""} },
        { "ADD",    {1, InstructionType::R, "r1, r2, r3"} },
        { "LDH",    {2, InstructionType::I, "r2, i(r1)"} },
        { "ADDI",   {3, InstructionType::I, "r1, i, r2"} },
        { "JUMP",   {4, InstructionType::J, "j"} },
        { "BRZ",    {5, InstructionType::I, "r1, j"} }
    },

    // opcode lookup
    {
        {0, "NOP"},
        {1, "ADD"},
        {2, "LDH"},
        {3, "ADDI"},
        {4, "JUMP"},
        {5, "BRZ"}
    }
};

struct InstructionEntry {
    quint32 encoded = 0;
    QString text = "NOP";
    std::shared_ptr<Instruction> instruction;
    bool valid = true;
    QString errorMessage;
};

class InstructionData : public QObject
{
    Q_OBJECT
public:

    InstructionSet instructionSet;
    QList<InstructionEntry> instructions;
    const int maxLines;
    quint32 baseAddress;
    explicit InstructionData(QObject *parent = nullptr, int maxLines = MAX_LINES, quint32 baseAddress = BASE_ADDRESS);

signals:
};

} // namespace InstructionEditor

#endif // INSTRUCTIONDATA_H
