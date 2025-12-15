#include "microcodedata.h"
#include "Common/hexint.h"

using namespace Microcode;

QStringList MicrocodeData::getValidStringValues(const size_t field){
    if(config.validValues.contains(field)){
        return config.validValues[field];
    }
    return {};
}

bool MicrocodeData::isValidStringValue(const size_t field, const QString &string, Qt::CaseSensitivity cs){
    if(string.isEmpty()){
        return true;
    }

    if(field == InstructionField::label){
        bool result = false;
        labelData.getAddress(string, &result);
        return !result;
    }
    else if(field == InstructionField::adr){
        bool result = false;
        labelData.getAddress(string, &result);
        return result;
    }
    else{
        QStringList validStrings = getValidStringValues(field);
        return validStrings.contains(string, cs);
    }
}

QString MicrocodeData::matchValidFieldValue(const size_t field, const QString &string, bool * okptr){
    QString trimmed = string.trimmed();

    if(field == InstructionField::label || field == InstructionField::adr){
        *okptr = true;
        return trimmed;
    }

    if(field == InstructionField::adr){
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

bool MicrocodeData::setValue(const size_t field, const size_t row, const QString &string){
    if(!isValidStringValue(field, string)){
        return false;
    }

    auto& instr = instructions[row];

    if(field == InstructionField::label){
        if(instr.label.isEmpty() && string.isEmpty()){
            // Do nothing
        }
        else if(!instr.label.isEmpty() && string.isEmpty()){
            // Remove label
            labelData.removeLabel(instr.label);
        }
        else if(instr.label.isEmpty() && !string.isEmpty()){
            // Create new label
            labelData.setLabel(string, row);
        }
        else if(!instr.label.isEmpty() && !string.isEmpty()){
            // Replace label
            labelData.removeLabel(instr.label);
            labelData.setLabel(string, row);
        }
        instr.label = string;
    }
    else if(field == InstructionField::adr){
        if(string.isEmpty()){
            instr.jumpAddress = NO_JUMP;
        }
        else{
            instr.jumpAddress = labelData.getAddress(string, nullptr);
        }

        instr.adr = string;
    }
    else{
        instr.setFieldValue(field, string);
    }
    return true;
}
