#ifndef INSTRUCTIONDEFINITION_H
#define INSTRUCTIONDEFINITION_H

#include <QString>
#include "tokenizer.h"
#include "InstructionEditor/instruction.h"

class InstructionDefinition
{
public:
    QString mnemonic;
    TokenList formatTokens;
    InstructionEditor::InstructionType type;

    InstructionDefinition();
    InstructionDefinition(QString mnemonic, InstructionEditor::InstructionType type, TokenList formatTokens);
    InstructionDefinition(QString mnemonic, InstructionEditor::InstructionType type, QString formatString);
    InstructionDefinition(QString definitionString, InstructionEditor::InstructionType type);
};

class InstructionSet{
private:
    QList<InstructionDefinition> definitions; // index in this list is the opcode
    QHash<QString, quint8> opcodeLookup;
public:
    InstructionSet(const QList<InstructionDefinition> &definitionList);
    const InstructionDefinition *getDefinition(quint8 opcode) const;
    const InstructionDefinition *getDefinition(QString mnemonic) const;
};

#endif // INSTRUCTIONDEFINITION_H
