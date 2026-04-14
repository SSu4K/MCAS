#ifndef INSTRUCTIONDEFINITION_H
#define INSTRUCTIONDEFINITION_H

#include "tokenizer.h"
#include "instruction.h"

namespace Assembly {

class InstructionDefinition
{
public:
    QString mnemonic;
    TokenList formatTokens;
    InstructionType type;

    InstructionDefinition(const InstructionDefinition& source);
    InstructionDefinition();
    InstructionDefinition(QString mnemonic, InstructionType type, TokenList formatTokens);
    InstructionDefinition(QString mnemonic, InstructionType type, QString formatString);
    InstructionDefinition(QString definitionString, InstructionType type);

    QString getFormatString() const;
};

extern const InstructionDefinition nopDefinition;
extern const InstructionDefinition emptyDefinition;

class InstructionSet{
private:
    QList<InstructionDefinition> definitions; // index in this list is the opcode
    QHash<QString, quint8> opcodeLookup;
public:
    InstructionSet();
    InstructionSet(const QList<InstructionDefinition> &definitionList);
    const InstructionDefinition *getDefinition(quint8 opcode) const;
    const InstructionDefinition *getDefinition(QString mnemonic) const;
    const quint8 getOpcode(const QString &mnemonic, bool *okptr) const;

    bool setDefinition(const quint8 opcode, const InstructionDefinition& definition);
    void removeDefinition(const quint8 opcode);
};

bool operator==(const InstructionDefinition &lhs, const InstructionDefinition &rhs);
bool operator!=(const InstructionDefinition &lhs, const InstructionDefinition &rhs);

}

#endif // INSTRUCTIONDEFINITION_H
