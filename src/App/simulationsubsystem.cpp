#include "simulationsubsystem.h"

using namespace Assembly;

SimulationSubsystem::SimulationSubsystem():
    microcodeConfig(),
    microcode(microcodeConfig),
    jumptable(),
    labelData(),
    instructionSet(),
    machineConfig(),
    breakpoints(machineConfig.instructionMemorySize/4),
    editorMachineState(machineConfig),
    engine(editorMachineState, microcode, jumptable, instructionSet, breakpoints)
{
    worker.setMachineState(&editorMachineState);
    worker.setExecutionEngine(&engine);
}
