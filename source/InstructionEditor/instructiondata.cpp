#include "instructiondata.h"

namespace InstructionEditor {

InstructionData::InstructionData(QObject *parent, int maxLines, quint32 baseAddress)
    : QObject{parent}, instructions(maxLines), maxLines(maxLines), baseAddress(baseAddress)
{
    instructionSet = DEFAULT_INSTRUCTION_SET;
}

} // namespace InstructionEditor
