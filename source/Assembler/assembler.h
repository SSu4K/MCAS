#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instructiondefinition.h"
#include "labeldata.h"
#include "assemblystatus.h"
#include "Common/twoscomplement.h"

namespace Assembly {

class Assembler
{
private:
    Tokenizer tokenizer;
    std::shared_ptr<const InstructionSet> instructionSet;
    std::shared_ptr<const LabelData> labelData;

    QMap<QString, Token> mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, AssemblyStatus &status) const;

    quint32 parseRegisterToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;
    quint32 parseHexToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;
    quint32 parseLabelToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;
    quint32 parseJumpToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;

    RType parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
    IType parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
    JType parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
public:

    Assembler(const std::shared_ptr<InstructionSet> &instructionSet, const std::shared_ptr<LabelData> &labelData);
    std::shared_ptr<Instruction> assembleLine(const QString &instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
};

}
#endif // ASSEMBLER_H
