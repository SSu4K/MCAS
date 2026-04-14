#include "modelssubsystem.h"

using namespace Models;

ModelsSubsystem::ModelsSubsystem(SimulationSubsystem &simulationSubsystem):
    simulation(simulationSubsystem),
    instructions(this, (int)simulationSubsystem.machineConfig.instructionMemorySize/4, 0),
    memoryModel(&simulationSubsystem.editorMachineState),
    instructionSetModel(&simulationSubsystem.instructionSet),
    instructionModel(&simulationSubsystem.editorMachineState,
                     &simulationSubsystem.labelData,
                     &simulationSubsystem.instructionSet,
                     &instructions,
                     nullptr),
    breakpointsModel(&simulationSubsystem.breakpoints, this),
    microcodeModel(&simulationSubsystem.microcode),
    jumpTableModel(&simulationSubsystem.jumptable)
{
    connect(&memoryModel, &MemoryModel::memoryRegionChanged, &instructionModel, &InstructionModel::onMemoryRegionChanged);
    connect(&instructionModel, &InstructionModel::memoryRegionChanged, &memoryModel, &MemoryModel::onMemoryRegionChanged);

    connect(&instructionSetModel, &InstructionSetModel::instructionSetChanged, &instructionModel, &InstructionModel::reassemble);
}
