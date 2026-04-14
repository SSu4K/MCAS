#ifndef SIMULATIONSUBSYSTEM_H
#define SIMULATIONSUBSYSTEM_H

#include "Assembly/instructiondefinition.h"
#include "Common/labeldata.h"
#include "Microcode/jumptabledata.h"
#include "Microcode/microcodedata.h"
#include "Machine/machinestate.h"
#include "breakpointdata.h"

#include "executionworker.h"

class SimulationSubsystem
{
public:
    SimulationSubsystem();

    Microcode::MicrocodeConfig microcodeConfig;
    Microcode::MicrocodeData microcode;
    Microcode::JumpTableData jumptable;
    LabelData labelData;
    Assembly::InstructionSet instructionSet;

    Machine::MachineConfig machineConfig;
    Machine::MachineState editorMachineState;

    BreakpointEditor::BreakpointData breakpoints;

    Sim::ExecutionEngine engine;
    Sim::ExecutionWorker worker;
};

#endif // SIMULATIONSUBSYSTEM_H
