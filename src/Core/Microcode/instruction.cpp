#include "instruction.h"

using namespace Microcode;

Instruction::Instruction(){
    address = "00000000";
    constant = "";
    jumpAddress = 0;
}

QString Instruction::getFieldValue(const size_t field) const{
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

bool Instruction::setFieldValue(const size_t field, const QString &value){
    switch(field){
    case InstructionField::address:     address = value; break;
    case InstructionField::label:       label = value; break;
    case InstructionField::alu:         alu = value; break;
    case InstructionField::s1:          s1 = value; break;
    case InstructionField::s2:          s2 = value; break;
    case InstructionField::dest:        dest = value; break;
    case InstructionField::jcond:       jcond = value; break;
    case InstructionField::adr:         adr = value; break;
    case InstructionField::constant:    constant = value; break;
    case InstructionField::extir:       extir = value; break;
    case InstructionField::mem:         mem = value; break;
    case InstructionField::madr:        madr = value; break;
    case InstructionField::mdest:       mdest = value; break;
    case InstructionField::regs:        regs = value; break;
    default:                            return false;
    }
    return true;
}

bool Instruction::isEmpty() const{
    return  label.isEmpty() && alu.isEmpty() && s1.isEmpty() && s2.isEmpty() &&
            dest.isEmpty() && jcond.isEmpty() && adr.isEmpty() && constant.isEmpty() &&
            extir.isEmpty() && mem.isEmpty() && madr.isEmpty() && mdest.isEmpty() && regs.isEmpty();
}
