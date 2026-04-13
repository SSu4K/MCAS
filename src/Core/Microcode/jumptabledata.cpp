#include "jumptabledata.h"
#include "Assembly/instruction.h"

using namespace Microcode;

JumpTableEntry::JumpTableEntry(){
    opcode = "";
    targets = {"", ""};
}

JumpTableData::JumpTableData(): entries(Assembly::encodingConfig->opcodeCount(), JumpTableEntry()){}

void JumpTableData::eraseAll(){
    entries = {Assembly::encodingConfig->opcodeCount(), JumpTableEntry()};
}
