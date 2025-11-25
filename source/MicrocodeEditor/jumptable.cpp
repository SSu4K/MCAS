#include "jumptable.h"

using namespace MicrocodeEditor;

JumpTableEntry::JumpTableEntry(){
    opcode = "";
    targets = {"", ""}; // temporary, works only for 2 jump tables.
}

JumpTable::JumpTable() {}
