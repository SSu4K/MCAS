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

const QString Instruction::matchValidFieldValue(const size_t field, const QString &string, bool * okptr){
    QString trimmed = string.trimmed();

    if(field == InstructionField::label || field == InstructionField::adr){
        *okptr = true;
        return trimmed;
    }

    if(field == InstructionField::address){
        // need to validate hex str
        const quint16 value = HexInt::hexStringToInt(trimmed, okptr);

        if(*okptr == false){
            return "";
        }

        return HexInt::intToString(value, false);
    }

    if(field == InstructionField::constant){
        if(trimmed.isEmpty()){
            *okptr = true;
            return "";
        }
        const quint16 value = HexInt::hexStringToInt(trimmed, okptr);

        if(*okptr == false){
            return "";
        }

        return HexInt::intToString(value, false, 1);
    }

    qsizetype index = validStringValues[field].indexOf(trimmed, 0, Qt::CaseInsensitive);
    if(index == -1){
        if(okptr) *okptr = false;
        return "";
    }

    if(okptr) *okptr = true;
    return validStringValues[field][index];
}

Instruction::Instruction(){
    address = HexInt::intToString(0, false);
    constant = "";
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

    bool ok = false;
    QString string = matchValidFieldValue(field, value.toString(), &ok);
    if(!ok) return false;

    switch(field){
        case InstructionField::address:     address = string; break;
        case InstructionField::label:       label = string; break;
        case InstructionField::alu:         alu = string; break;
        case InstructionField::s1:          s1 = string; break;
        case InstructionField::s2:          s2 = string; break;
        case InstructionField::dest:        dest = string; break;
        case InstructionField::jcond:       jcond = string; break;
        case InstructionField::adr:         adr = string; break;
        case InstructionField::constant:    constant = string; break;
        case InstructionField::extir:       extir = string; break;
        case InstructionField::mem:         mem = string; break;
        case InstructionField::madr:        madr = string; break;
        case InstructionField::mdest:       mdest = string; break;
        case InstructionField::regs:        regs = string; break;
        default:                            return false;
    }
    return true;
}

Microcode::Microcode() {
    // You can initialize some default microinstructions if needed.
    // For now, it's just empty.
}
