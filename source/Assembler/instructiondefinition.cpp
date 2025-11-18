#include "instructiondefinition.h"

static Tokenizer formatTokenizer;

InstructionDefinition::InstructionDefinition()
    : mnemonic(""), type(InstructionType::None), formatTokens({}) {}

InstructionDefinition::InstructionDefinition(QString mnemonic, InstructionType type, TokenList formatTokens)
    : mnemonic(mnemonic), type(type), formatTokens(formatTokens) {}

InstructionDefinition::InstructionDefinition(QString mnemonic, InstructionType type, QString formatString)
    : mnemonic(mnemonic), type(type)
{
    auto tokens = formatTokenizer.tokenizeLine(formatString, 0);
    // TODO: validation in future
    formatTokens = tokens;
}

InstructionDefinition::InstructionDefinition(QString definitionString, InstructionType type)
    : type(type)
{
    auto tokens = formatTokenizer.tokenizeLine(definitionString, 0);
    // TODO: validation in future
    mnemonic = tokens[0].text;
    tokens.pop_front();
    formatTokens = tokens;
}

InstructionSet::InstructionSet(const QList<InstructionDefinition> &definitionList){
    definitions = definitionList;
    for(qsizetype i=0; i<definitionList.size(); i++){
        opcodeLookup[definitionList[i].mnemonic] = i;
    }
}

const InstructionDefinition *InstructionSet::getDefinition(quint8 opcode) const{
    if(opcode >= definitions.size()){
        return nullptr;
    }
    return &definitions[opcode];
}

const InstructionDefinition *InstructionSet::getDefinition(QString mnemonic) const{
    if(!opcodeLookup.contains(mnemonic)){
        return nullptr;
    }
    quint8 opcode = opcodeLookup[mnemonic];
    return &definitions[opcode];
}

const quint8 InstructionSet::getOpcode(const QString &mnemonic, bool *okptr) const{
    if(!opcodeLookup.contains(mnemonic)){
        if(okptr) *okptr = false;
        return 0;
    }
    if(okptr) *okptr = true;
    return opcodeLookup[mnemonic];
}
