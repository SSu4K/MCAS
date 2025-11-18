#include "instruction.h"
#include "Common/hexint.h"


InstructionEncodingConig* encodingConfig = new InstructionEncodingConig(); // local for now

void InstructionEncodingConig::updateValues(){
    opcodeCount_ = 1 << opcodeSize_;
    opcodeMask_ = opcodeCount_ - 1;

    registerCount_ = 1 << registerSize_;
    registerMask_ = registerCount_ - 1;

    const quint8 argumentsSize = 32 - opcodeSize_;

    RFormalCount_ = argumentsSize / registerSize_;
    RUnusedSize_ = argumentsSize % registerSize_;

    IImmediateSize_ = (argumentsSize - 2*registerSize_);
    IImmediateMask_ = (1 << IImmediateSize_) - 1;
    IUnusedSize_ = 0;

    JImmediateSize_ = argumentsSize;
    JImmediateMask_ = (1 << JImmediateSize_) - 1;
    JUnusedSize_ = 0;
}

InstructionEncodingConig::InstructionEncodingConig(quint8 opcodeSize, quint8 registerSize)
{
    if(opcodeSize < 4 || opcodeSize > 8){
        opcodeSize_ = opcodeSize;
    }
    else{
        opcodeSize_ = DEFAULT_OPCODE_SIZE;
    }

    if(registerSize < 2 || registerSize > 8){
        registerSize_ = registerSize;
    }
    else{
        registerSize_ = DEFAULT_REGISTER_SIZE;
    }

    updateValues();

}

bool InstructionEncodingConig::setOpcodeSize(quint8 size){
    if(size < 4 || size > 8) return false;
    opcodeSize_ = size;
    updateValues();

    return true;
}

bool InstructionEncodingConig::setRegisterSize(quint8 size){
    if(size < 2 || size > 8) return false;
    registerSize_ = size;
    updateValues();

    return true;
}

RType::RType(){
    opcode_ = 0;
    formals.fill(0, encodingConfig->RFormalCount());
}

RType::RType(const quint8 opcode, QList<quint8> formals){
    this->opcode_= opcode;
    for(qsizetype i=0; i<encodingConfig->RFormalCount(); i++){
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
    result |= encodingConfig->opcodeMask() & opcode_;

    for(qsizetype i=0; i<encodingConfig->RFormalCount(); i++){
        result = result << encodingConfig->registerSize();
        result |= encodingConfig->registerMask() & formals[i];
    }
    return result << encodingConfig->RUnusedSize();
}

RType RType::decode(quint32 in){
    RType result;
    in = in >> encodingConfig->RUnusedSize();
    for(qsizetype i = encodingConfig->RFormalCount()-1; i>=0; i--){
        qint8 formal = in & encodingConfig->registerMask();
        result.formals[i] = formal;
        in = in >> encodingConfig->registerSize();
    }

    result.opcode_= encodingConfig->opcodeMask() & in;

    return result;
}

IType::IType(quint8 opcode, quint8 source, quint8 destination, quint16 immediate){
    this->opcode_= opcode;
    this->sourceRegister = source;
    this->destinationRegister = destination;
    this->immediate = immediate;
}

quint32 IType::encode() const{
    quint32 result = 0;
    result |= encodingConfig->opcodeMask() & opcode_;

    result = result << encodingConfig->registerSize();
    result |= encodingConfig->registerMask() & sourceRegister;

    result = result << encodingConfig->registerSize();
    result |= encodingConfig->registerMask() & destinationRegister;

    result = result << encodingConfig->IImmediateSize();
    result |= encodingConfig->IImmediateMask() & immediate;

    return result << encodingConfig->IUnusedSize();
}

IType IType::decode(quint32 in){
    IType result;
    in = in >> encodingConfig->IUnusedSize();

    result.immediate = encodingConfig->IImmediateMask() & in;
    in = in >> encodingConfig->IImmediateSize();

    result.destinationRegister = encodingConfig->registerMask() & in;
    in = in >> encodingConfig->registerSize();

    result.sourceRegister = encodingConfig->registerMask() & in;
    in = in >> encodingConfig->registerSize();

    result.opcode_= encodingConfig->opcodeMask() & in;
    return result;
}

QString IType::toString() const{
    QString hexImmediate = HexInt::intToString(immediate, true, qsizetype(encodingConfig->IImmediateSize()/4));
    return QString("%1, %2, %3, ").arg(opcode_).arg(sourceRegister).arg(destinationRegister) + hexImmediate;
}

JType::JType(const quint8 opcode, const quint32 immediate){
    this->opcode_= opcode;
    this->immediate = immediate;
}

quint32 JType::encode() const{
    quint32 result = 0;
    result |= encodingConfig->opcodeMask() & opcode_;

    result = result << encodingConfig->JImmediateSize();
    result |= encodingConfig->JImmediateMask() & immediate;

    return result << encodingConfig->JUnusedSize();
}

JType JType::decode(quint32 in){
    JType result;
    in = in >> encodingConfig->JUnusedSize();

    result.immediate = encodingConfig->JImmediateMask() & in;
    in = in >> encodingConfig->JImmediateSize();

    result.opcode_= encodingConfig->opcodeMask() & in;
    return result;
}

bool operator==(const RType &l, const RType &r){
    if(l.opcode_!= r.opcode_){
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

bool operator==(const IType &l, const IType &r){
    if(l.opcode_!= r.opcode_){
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

bool operator==(const JType &l, const JType &r){
    if(l.opcode_!= r.opcode_){
        return false;
    }

    return l.immediate == r.immediate;
}
