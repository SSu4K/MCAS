#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "assemblystatus.h"
#include "tokenizer.h"

class LabelData;

namespace Assembly {

class Instruction;
class InstructionSet;
class InstructionDefinition;
class RType;
class IType;
class JType;
class Token;

class Assembler
{
private:
    Tokenizer tokenizer;
    const InstructionSet* instructionSet;
    const LabelData* labelData;

    QMap<QString, Token> mapTokens(const TokenList &formatTokens, const TokenList &argumentTokens, AssemblyStatus &status) const;

    quint32 parseRegisterToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;
    quint32 parseHexToken(const Token &token, const qsizetype bitWidth, AssemblyStatus &status) const;
    quint32 parseLabelToken(const Token &token, const qsizetype bitWidth, const bool isRelative, AssemblyStatus &status) const;
    quint32 parseJumpToken(const Token &token, const qsizetype bitWidth, const bool isRelative, AssemblyStatus &status) const;

    RType parseRType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
    IType parseIType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
    JType parseJType(const QMap<QString, Token> &tokenMappings, const InstructionDefinition *definition, AssemblyStatus &status) const;
public:

    Assembler(const InstructionSet* instructionSet, const LabelData* labelData);
    std::shared_ptr<Instruction> assembleLine(const QString &instruction, const qsizetype lineNumber, AssemblyStatus &status) const;
};

}
#endif // ASSEMBLER_H
