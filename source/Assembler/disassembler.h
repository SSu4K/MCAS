#ifndef DISASSEMBLER_H
#define DISASSEMBLER_H

#include "assemblystatus.h"
#include "instructiondefinition.h"
#include "labeldata.h"
#include "Common/hexint.h"
#include "Common/twoscomplement.h"

namespace Assembly {

class Disassembler
{
private:
    std::shared_ptr<const InstructionSet> instructionSet;
    std::shared_ptr<const LabelData> labelData;

    QString assembleRType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
    QString assembleIType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
    QString assembleJType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;

public:
    Disassembler(const std::shared_ptr<InstructionSet> &instructionSet, const std::shared_ptr<LabelData> &labelData);
    QString assembleLine(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
};

} // namespace Assembly

#endif // DISASSEMBLER_H
