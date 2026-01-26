#include "instructiondefinition.h"

using namespace Assembly;

const InstructionDefinition Assembly::nopDefinition = {"NOP", InstructionType::R, TokenList()};
const InstructionDefinition Assembly::emptyDefinition = {"", InstructionType::None, TokenList()};

InstructionDefinition::InstructionDefinition(const InstructionDefinition& source)
    : mnemonic(source.mnemonic), type(source.type), formatTokens(source.formatTokens) {}

// the same as 'emptyDefinition'
InstructionDefinition::InstructionDefinition()
    : InstructionDefinition(emptyDefinition) {}

InstructionDefinition::InstructionDefinition(QString mnemonic, InstructionType type, TokenList formatTokens)
    : mnemonic(mnemonic), type(type), formatTokens(formatTokens) {}

InstructionDefinition::InstructionDefinition(QString mnemonic, InstructionType type, QString formatString)
    : mnemonic(mnemonic), type(type)
{
    Tokenizer formatTokenizer;
    auto tokens = formatTokenizer.tokenizeLine(formatString, 0);
    // TODO: validation in future
    formatTokens = tokens;
}

InstructionDefinition::InstructionDefinition(QString definitionString, InstructionType type)
    : type(type)
{
    Tokenizer formatTokenizer;
    auto tokens = formatTokenizer.tokenizeLine(definitionString, 0);
    // TODO: validation in future
    mnemonic = tokens[0].text;
    tokens.pop_front();
    formatTokens = tokens;
}

QString InstructionDefinition::getFormatString() const{
    if(formatTokens.size() < 1) return "";
    QString result = formatTokens[0].text;
    bool inBracket = false;
    for(qsizetype i=1; i<formatTokens.size(); i++){
        if(!inBracket) result.append(", ");
        result.append(formatTokens[i].text);
        if(formatTokens[i].text == "(") inBracket = true;
        else if(formatTokens[i].text == ")") inBracket = false;
    }

    return result;
}

InstructionSet::InstructionSet(): definitions(encodingConfig->opcodeCount(), InstructionDefinition()){
    // reserve opcode = 0 for NOP
    definitions[0] = nopDefinition;
    opcodeLookup[nopDefinition.mnemonic] = 0;
}

InstructionSet::InstructionSet(const QList<InstructionDefinition> &definitionList): InstructionSet(){
    definitions = definitionList;

    // always skip 0th definition, resrved for NOP
    for(qsizetype i=1; i<definitionList.size(); i++){
        definitions[i] = definitionList[i];
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

bool InstructionSet::setDefinition(const quint8 opcode, const InstructionDefinition& definition){
    // protect out of bounds and reserved NOP instruction definition
    if(opcode >= definitions.size() || opcode == 0){
        return false;
    }
    removeDefinition(opcode);

    definitions[opcode] = definition;
    opcodeLookup[definition.mnemonic] = opcode;
    return true;
}

void InstructionSet::removeDefinition(const quint8 opcode){
    // protect out of bounds and reserved NOP instruction definition
    if(opcode >= definitions.size() || opcode == 0){
        return;
    }
    opcodeLookup.remove(definitions[opcode].mnemonic);
    definitions[opcode] = emptyDefinition;
}

bool Assembly::operator==(const InstructionDefinition &lhs, const InstructionDefinition &rhs){
    if(lhs.type != rhs.type) return false;
    if(lhs.mnemonic.compare(rhs.mnemonic, Qt::CaseInsensitive) == 0) return false;
    if(lhs.formatTokens.size() != rhs.formatTokens.size()) return false;

    for(qsizetype i=0; i<lhs.formatTokens.size(); i++){
        if(lhs.formatTokens[i].text == rhs.formatTokens[i].text) return false;
    }

    return true;
}

bool Assembly::operator!=(const InstructionDefinition &lhs, const InstructionDefinition &rhs){
    return !(lhs == rhs);
}
