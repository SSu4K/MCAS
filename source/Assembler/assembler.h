#ifndef ASSEMBLER_H
#define ASSEMBLER_H

#include "instructiondefinition.h"

class Assembler
{
private:
    std::shared_ptr<const InstructionSet> instructionSet;

public:
    Assembler();
};

#endif // ASSEMBLER_H
