#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "assemblystatus.h"

namespace Assembly {

// forward declarations
class InstructionSet;
class LabelData;

class Disassembler
{
private:
    std::shared_ptr<const InstructionSet> instructionSet;
    std::shared_ptr<const LabelData> labelData;

    QString disassembleRType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
    QString disassembleIType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
    QString disassembleJType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;

public:
    Disassembler(const std::shared_ptr<InstructionSet> &instructionSet, const std::shared_ptr<LabelData> &labelData);
    QString disassembleLine(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
};

} // namespace Assembly

#endif // DISASSEMBLER_H
