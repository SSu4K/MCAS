#include "disassembler.h"

using namespace Assembly;

QString Disassembler::disassembleRType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const {
    RType r = RType::decode(instruction);
    auto definition = instructionSet->getDefinition(r.opcode());
    QString result  = definition->mnemonic;

    for(auto &token : definition->formatTokens){
        int registerIndex = token.text.sliced(1).toInt()-1; // format register number starts ar r1 therefore: -1
        result += QString(" R%1").arg(r.formals[registerIndex]);
    }

    return result;
}

QString Disassembler::disassembleIType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const {
    IType i = IType::decode(instruction);
    auto definition = instructionSet->getDefinition(i.opcode());
    QString result  = definition->mnemonic;

    bool inBracket = false;
    for(auto &token : definition->formatTokens){
        if(token.type == TokenType::Register){
            int registerIndex = token.text.sliced(1).toInt();
            if(registerIndex == 1){
                if(!inBracket) result += " ";
                result += QString("R%1").arg(i.sourceRegister);
            }
            else if(registerIndex == 2){
                if(!inBracket) result += " ";
                result += QString("R%1").arg(i.destinationRegister);
            }
        }
        else if(token.type == TokenType::Bracket){
            inBracket = (token.text == "(");
            result += token.text;
        }
        else if(token.type == TokenType::Identifier){
            if(token.text == "i"){
                result += " ";
                result += HexInt::intToString(i.immediate, true, 4);
            }
            else if (token.text == "j"){
                quint32 base = 4*(lineNumber+1);
                qint32 offset = TwoC::toInt(i.immediate, encodingConfig->IImmediateSize());
                quint32 address = base + offset;

                bool ok;
                QString label = labelData->getLabel(address, &ok);
                if(ok){
                    result += " ";
                    result += label;
                }
                else{
                    result += " ";
                    result += HexInt::intToString(i.immediate, true, 4);
                }

            }
        }
    }
    return result;
}

QString Disassembler::disassembleJType(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const {
    JType j = JType::decode(instruction);
    auto definition = instructionSet->getDefinition(j.opcode());
    QString result  = definition->mnemonic;

    bool inBracket = false;
    for(auto &token : definition->formatTokens){
        if(token.type == TokenType::Identifier && token.text == "j"){
            quint32 address = j.immediate;

            bool ok;
            QString label = labelData->getLabel(address, &ok);
            if(ok){
                result += " ";
                result += label;
            }
            else{
                result += " ";
                result += HexInt::intToString(j.immediate, true, 4);
            }

        }
    }
    return result;
}

Disassembler::Disassembler(const std::shared_ptr<InstructionSet> &instructionSet, const std::shared_ptr<LabelData> &labelData)
    : instructionSet(instructionSet), labelData(labelData) {}

QString Disassembler::disassembleLine(const quint32 instruction, const qsizetype lineNumber, AssemblyStatus &status) const{
    quint32 opcode = encodingConfig->opcodeMask() & (instruction >> (32 - encodingConfig->opcodeSize()));
    auto definition = instructionSet->getDefinition(opcode);

    if(definition == nullptr){
        status = AssemblyStatus::fail(ErrorType::InvalidToken, "Invalid opcode");
        return "";
    }
    auto type = definition->type;

    QString result;
    switch(type){
    case InstructionType::R: result = disassembleRType(instruction, lineNumber, status); break;
    case InstructionType::I: result = disassembleIType(instruction, lineNumber, status); break;
    case InstructionType::J: result = disassembleJType(instruction, lineNumber, status); break;
    case InstructionType::None: result = ""; break;
    }

    return result;
}
