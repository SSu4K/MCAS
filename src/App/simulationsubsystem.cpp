#include "simulationsubsystem.h"

using namespace Assembly;

SimulationSubsystem::SimulationSubsystem():
    microcodeConfig(),
    microcode(microcodeConfig),
    jumptable(),
    labelData(),
    instructionSet(),
    machineConfig(),
    editorMachineState(machineConfig),
    engine(editorMachineState, microcode, jumptable, instructionSet)
{
    worker.setMachineState(&editorMachineState);
    worker.setExecutionEngine(&engine);
}
