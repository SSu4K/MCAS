#include "modelssubsystem.h"

using namespace Models;

ModelsSubsystem::ModelsSubsystem(SimulationSubsystem &simulationSubsystem):
    instructions(this, (int)simulationSubsystem.machineConfig.instructionMemorySize, 0),
    memoryModel(&simulationSubsystem.editorMachineState),
    instructionModel(&simulationSubsystem.editorMachineState,
                     &simulationSubsystem.labelData,
                     &simulationSubsystem.instructionSet,
                     &instructions,
                     ),
    microcodeModel(&simulationSubsystem.microcode),
    jumpTableModel(&simulationSubsystem.jumptable)
{
    connect(&memoryModel, &MemoryModel::memoryRegionChanged, &instructionModel, &InstructionModel::onMemoryRegionChanged);
    connect(&instructionModel, &InstructionModel::memoryRegionChanged, &memoryModel, &MemoryModel::onMemoryRegionChanged);
}
