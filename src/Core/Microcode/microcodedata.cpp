#include "microcodedata.h"
#include "Common/hexint.h"

using namespace Microcode;

// const QMap<size_t, QString> Microcode::Instruction::fieldNames = {
//     {InstructionField::address,     "uAR"},
//     {InstructionField::label,       "Label"},
//     {InstructionField::alu,         "ALU"},
//     {InstructionField::s1,          "S1"},
//     {InstructionField::s2,          "S2"},
//     {InstructionField::dest,        "Dest"},
//     {InstructionField::extir,       "ExtIR"},
//     {InstructionField::constant,    "Const"},
//     {InstructionField::jcond,       "JCond"},
//     {InstructionField::adr,         "Adr"},
//     {InstructionField::mem,         "Mem"},
//     {InstructionField::madr,        "MAdr"},
//     {InstructionField::mdest,       "MDest"},
//     {InstructionField::regs,        "Regs"}
// };

// const QMap<size_t, QStringList> Microcode::Instruction::validStringValues = {
//     {InstructionField::alu,     aluOptions},
//     {InstructionField::s1,      s1Options},
//     {InstructionField::s2,      s2Options},
//     {InstructionField::dest,    destOptions},
//     {InstructionField::extir,   extirOptions},
//     {InstructionField::jcond,   jcondOptions},
//     {InstructionField::mem,     memOptions},
//     {InstructionField::madr,    madrOptions},
//     {InstructionField::mdest,   mdestOptions},
//     {InstructionField::regs,    regsOptions}
// };

QStringList MicrocodeData::getValidStringValues(const size_t field){
    if(config.validValues.contains(field)){
        return config.validValues[field];
    }
    return {};
}

bool MicrocodeData::isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs){
    QStringList validStrings = getValidStringValues(field);
    return validStrings.contains(string, cs);
}

QString MicrocodeData::matchValidFieldValue(const size_t field, const QString &string, bool * okptr){
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

    qsizetype index = config.validValues[field].indexOf(trimmed, 0, Qt::CaseInsensitive);
    if(index == -1){
        if(okptr) *okptr = false;
        return "";
    }

    if(okptr) *okptr = true;
    return config.validValues[field][index];
}

MicrocodeData::MicrocodeData(const MicrocodeConfig &config) : config(config) {
}
