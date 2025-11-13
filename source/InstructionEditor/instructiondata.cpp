#include "instructiondata.h"

namespace InstructionEditor {

InstructionData::InstructionData(QObject *parent, int maxLines, quint32 baseAddress)
    : QObject{parent}, maxLines(maxLines), baseAddress(baseAddress)
{}

} // namespace InstructionEditor
