#ifndef MODELSSUBSYSTEM_H
#define MODELSSUBSYSTEM_H

#include "simulationsubsystem.h"

#include "instructiondata.h"
#include "Memory/memorymodel.h"
#include "Instruction/instructionmodel.h"
#include "Microcode/microcodemodel.h"
#include "Microcode/jumptablemodel.h"

class ModelsSubsystem: QObject
{
    Q_OBJECT
public:
    ModelsSubsystem(SimulationSubsystem &simulationSubsystem);

    SimulationSubsystem &simulation;
    InstructionEditor::InstructionData instructions;
    Models::MemoryModel memoryModel;
    Models::InstructionModel instructionModel;
    Models::MicrocodeModel microcodeModel;
    Models::JumpTableModel jumpTableModel;
};

#endif // MODELSSUBSYSTEM_H
