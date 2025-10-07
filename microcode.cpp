#include "microcode.h"
#include "hexint.h"

using namespace MicrocodeEditor;

const QMap<size_t, QString> Instruction::fieldNames = {
    {InstructionField::address,     "uAR"},
    {InstructionField::label,       "Label"},
    {InstructionField::alu,         "ALU"},
    {InstructionField::s1,          "S1"},
    {InstructionField::s2,          "S2"},
    {InstructionField::dest,        "Dest"},
    {InstructionField::extir,       "ExtIR"},
    {InstructionField::constant,    "Const"},
    {InstructionField::jcond,       "JCond"},
    {InstructionField::adr,         "Adr"},
    {InstructionField::mem,         "Mem"},
    {InstructionField::madr,        "MAdr"},
    {InstructionField::mdest,       "MDest"},
    {InstructionField::regs,        "Regs"}
};

const QMap<size_t, QStringList> Instruction::validStringValues = {
    {InstructionField::alu,     aluOptions},
    {InstructionField::s1,      s1Options},
    {InstructionField::s2,      s2Options},
    {InstructionField::dest,    destOptions},
    {InstructionField::extir,   extirOptions},
    {InstructionField::jcond,   jcondOptions},
    {InstructionField::mem,     memOptions},
    {InstructionField::madr,    madrOptions},
    {InstructionField::mdest,   mdestOptions},
    {InstructionField::regs,    regsOptions}
};

const QStringList Instruction::getValidStringValues(const size_t field){
    if(validStringValues.contains(field)){
        return validStringValues[field];
    }
    return {};
}

const bool Instruction::isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs){
    QStringList validStrings = getValidStringValues(field);
    return validStrings.contains(string, cs);
}

Instruction::Instruction(){
    address = HexInt::intToString(0);
    constant = HexInt::intToString(0);
}

QVariant Instruction::getFieldValue(const size_t field) const{
    switch(field){
        case InstructionField::address:     return address;
        case InstructionField::label:       return label;
        case InstructionField::alu:         return alu;
        case InstructionField::s1:          return s1;
        case InstructionField::s2:          return s2;
        case InstructionField::dest:        return dest;
        case InstructionField::jcond:       return jcond;
        case InstructionField::adr:         return adr;
        case InstructionField::constant:    return constant;
        case InstructionField::extir:       return extir;
        case InstructionField::mem:         return mem;
        case InstructionField::madr:        return madr;
        case InstructionField::mdest:       return mdest;
        case InstructionField::regs:        return regs;
        default:                            return {};
    }
}

bool Instruction::setFieldValue(const size_t field, const QVariant &value){
    switch(field){
        case InstructionField::address:     address = value.toString(); break;
        case InstructionField::label:       label = value.toString(); break;
        case InstructionField::alu:         alu = value.toString(); break;
        case InstructionField::s1:          s1 = value.toString(); break;
        case InstructionField::s2:          s2 = value.toString(); break;
        case InstructionField::dest:        dest = value.toString(); break;
        case InstructionField::jcond:       jcond = value.toString(); break;
        case InstructionField::adr:         adr = value.toString(); break;
        case InstructionField::constant:    constant = value.toString(); break;
        case InstructionField::extir:       extir = value.toString(); break;
        case InstructionField::mem:         mem = value.toString(); break;
        case InstructionField::madr:        madr = value.toString(); break;
        case InstructionField::mdest:       mdest = value.toString(); break;
        case InstructionField::regs:        regs = value.toString(); break;
        default:                            return false;
    }
    return true;
}

Microcode::Microcode() {
    // You can initialize some default microinstructions if needed.
    // For now, it's just empty.
}
