#include "instruction.h"

using namespace InstructionEditor;

RType::RType(quint8 opcode, QList<quint8> formals){
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

quint32 RType::encode(){
    quint32 result = 0;
    result |= OPCODE_MASK & opcode;
    result = result << OPCODE_SIZE;

    for(qsizetype i=0; i<R_FORMAL_COUNT; i++){
        result = result << REGISTER_SIZE;
        result |= REGISTER_MASK & formals[i];
    }
    return result << R_UNUSED_SIZE;
}

IType::IType(quint8 opcode, quint8 source, quint8 destination, quint16 immediate){
    this->opcode = opcode;
    this->sourceRegister = source;
    this->destinationRegister = destination;
    this->immediate = immediate;
}

quint32 IType::encode(){
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

JType::JType(quint8 opcode, quint32 immediate){
    this->opcode = opcode;
    this->immediate = immediate;
}

quint32 JType::encode(){
    quint32 result = 0;
    result |= OPCODE_MASK & opcode;

    result = result << I_IMMEDIATE_SIZE;
    result |= I_IMMEDIATE_MASK & immediate;

    return result << I_UNUSED_SIZE;
}
