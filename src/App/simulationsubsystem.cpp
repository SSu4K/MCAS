#include "simulationsubsystem.h"

using namespace Assembly;

SimulationSubsystem::SimulationSubsystem():
    microcodeConfig(),
    microcode(microcodeConfig),
    jumptable(),
    labelData(),
    instructionSet({
      { "NOP",    InstructionType::R, ""},
      { "ADD",    InstructionType::R, "r1, r2, r3"},
      { "LDH",    InstructionType::I, "r2, i(r1)"},
      { "ADDI",   InstructionType::I, "r1, i, r2"},
      { "JUMP",   InstructionType::J, "j"},
      { "BRZ",    InstructionType::I, "r1, j"},
      }),
    machineConfig(),
    editorMachineState(machineConfig)
{}
