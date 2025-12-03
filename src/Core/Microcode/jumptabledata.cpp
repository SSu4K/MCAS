#include "jumptabledata.h"

using namespace Microcode;

JumpTableEntry::JumpTableEntry(){
    opcode = "";
    targets = {"", ""}; // temporary, works only for 2 jump tables.
}

JumpTableData::JumpTableData() {}
