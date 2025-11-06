#include "instruction.h"

using namespace InstructionEditor;

RType::RType(){
    opcode = 0;
    formals.fill(0, R_FORMAL_COUNT);
}

RType::RType(const quint8 opcode, QList<quint8> formals){
    this->opcode = opcode;
    for(qsizetype i=0; i<R_FORMAL_COUNT; i++){
        if(i<formals.count()){
            this->formals.append(formals[i]);
        }
        else{
            this->formals.append(0);
        }
    }
}

quint32 RType::encode() const{
    quint32 result = 0;
    result |= OPCODE_MASK & opcode;
    result = result << OPCODE_SIZE;

    for(qsizetype i=0; i<R_FORMAL_COUNT; i++){
        result = result << REGISTER_SIZE;
        result |= REGISTER_MASK & formals[i];
    }
    return result << R_UNUSED_SIZE;
}

RType RType::decode(quint32 in){
    RType result;
    in = in >> R_UNUSED_SIZE;
    for(qsizetype i = R_FORMAL_COUNT-1; i>=0; i--){
        qint8 formal = in & REGISTER_MASK;
        result.formals[i] = formal;
        in = in >> REGISTER_SIZE;
    }

    result.opcode = OPCODE_MASK & in;

    return result;
}

IType::IType(quint8 opcode, quint8 source, quint8 destination, quint16 immediate){
    this->opcode = opcode;
    this->sourceRegister = source;
    this->destinationRegister = destination;
    this->immediate = immediate;
}

quint32 IType::encode() const{
    quint32 result = 0;
    result |= OPCODE_MASK & opcode;

    result = result << OPCODE_SIZE;
    result |= REGISTER_MASK & sourceRegister;

    result = result << REGISTER_SIZE;
    result |= REGISTER_MASK & destinationRegister;

    result = result << I_IMMEDIATE_SIZE;
    result |= I_IMMEDIATE_MASK & immediate;

    return result << I_UNUSED_SIZE;
}

IType IType::decode(quint32 in){
    IType result;
    in = in >> I_UNUSED_SIZE;

    result.immediate = I_IMMEDIATE_MASK & in;
    in = in >> I_IMMEDIATE_SIZE;

    result.destinationRegister = REGISTER_MASK & in;
    in = in >> REGISTER_SIZE;

    result.sourceRegister = REGISTER_MASK & in;
    in = in >> REGISTER_SIZE;

    result.opcode = OPCODE_MASK & in;
    return result;
}

JType::JType(const quint8 opcode, const quint32 immediate){
    this->opcode = opcode;
    this->immediate = immediate;
}

quint32 JType::encode() const{
    quint32 result = 0;
    result |= OPCODE_MASK & opcode;

    result = result << I_IMMEDIATE_SIZE;
    result |= I_IMMEDIATE_MASK & immediate;

    return result << I_UNUSED_SIZE;
}

JType JType::decode(quint32 in){
    JType result;
    in = in >> J_UNUSED_SIZE;

    result.immediate = J_IMMEDIATE_MASK & in;
    in = in >> J_IMMEDIATE_SIZE;

    result.opcode = OPCODE_MASK & in;
    return result;
}

bool InstructionEditor::operator==(const RType &l, const RType &r){
    if(l.opcode != r.opcode){
        return false;
    }

    if(l.formals.size() != r.formals.size()){
        return false;
    }
    for(qsizetype i=0; i<l.formals.size(); i++){
        if(l.formals[i] != r.formals[i]){
            return false;
        }
    }

    return true;
}

bool InstructionEditor::operator==(const IType &l, const IType &r){
    if(l.opcode != r.opcode){
        return false;
    }
    if(l.sourceRegister != r.sourceRegister){
        return false;
    }
    if(l.destinationRegister != r.destinationRegister){
        return false;
    }

    return l.immediate == r.immediate;
}

bool InstructionEditor::operator==(const JType &l, const JType &r){
    if(l.opcode != r.opcode){
        return false;
    }

    return l.immediate == r.immediate;
}
