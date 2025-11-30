#include "microcodeconfig.h"
#include "instruction.h"

using namespace Microcode;

static const QString emptyOptionString = "";
static const QStringList aluOptions    = {emptyOptionString, "ADD", "SUB", "RSUB", "AND", "OR", "XOR", "SSL", "SRA", "S1", "S2", "S2S1", "MUL", "DIV"};
static const QStringList s1Options     = {emptyOptionString, "A", "Const", "PC", "MAR", "MDR", "IR"};
static const QStringList s2Options     = {emptyOptionString, "B", "Const", "PC", "MAR", "MDR", "IR"};
static const QStringList destOptions   = {emptyOptionString, "C", "PC", "MAR", "MDR"};
static const QStringList jcondOptions  = {emptyOptionString, "True", "EQ", "NE", "LT", "GT", "LE", "GE", "MBusy", "Jump1", "Jump2"};
static const QStringList extirOptions  = {emptyOptionString, "Byte", "Half", "Word"};
static const QStringList memOptions    = {emptyOptionString, "RB", "RH", "RW", "WB", "WH", "WW"};
static const QStringList madrOptions   = {emptyOptionString, "MAR", "PC"};
static const QStringList mdestOptions  = {emptyOptionString, "MAR", "PC"};
static const QStringList regsOptions   = {
    emptyOptionString, "RR",
    "RAF1", "RAF2", "RAF3",
    "RBF1", "RBF2", "RBF3",
    "WF1", "WF2", "WF3",
    "RAA1", "RAA2", "RAA3",
    "RBA1", "RBA2", "RBA3",
    "WA1", "WA2", "WA3"
};

MicrocodeConfig::MicrocodeConfig(){
    fieldNames = {
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

    validValues = {
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

}

QStringList MicrocodeConfig::getValidValues(size_t field) const {
    return validValues.value(field);
}

QString MicrocodeConfig::getFieldName(size_t field) const {
    return fieldNames.value(field);
}
