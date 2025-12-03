#ifndef SIMULATIONSUBSYSTEM_H
#define SIMULATIONSUBSYSTEM_H

#include "Assembly/instructiondefinition.h"
#include "Assembly/labeldata.h"
#include "Microcode/jumptabledata.h"
#include "Microcode/microcodedata.h"
#include "Machine/machinestate.h"

class SimulationSubsystem
{
public:
    SimulationSubsystem();

    Microcode::MicrocodeConfig microcodeConfig;
    Microcode::MicrocodeData microcode;
    Microcode::JumpTableData jumptable;
    Assembly::LabelData labelData;
    Assembly::InstructionSet instructionSet;

    Machine::MachineConfig machineConfig;
    Machine::MachineState editorMachineState;
};

#endif // SIMULATIONSUBSYSTEM_H
