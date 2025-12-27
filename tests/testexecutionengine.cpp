#include "testexecutionengine.h"

#include "Simulation/executionengine.h"
#include "Simulation/effects.h"

#include "Machine/machinestate.h"

#include "Microcode/instruction.h"
#include "Microcode/instruction.h"

using Sim::ExecutionEngine;
using Sim::Effects;

using Assembly::InstructionSet;
using Assembly::InstructionDefinition;
using Assembly::InstructionType;

static const InstructionSet testInstructionSet({
    InstructionDefinition("NOP",    InstructionType::R, ""),
    InstructionDefinition("ADD",    InstructionType::R, "r1, r2, r3"),
    InstructionDefinition("SUB",    InstructionType::R, "r1, r2, r3"),
    InstructionDefinition("LDH",    InstructionType::I, "r2, i(r1)"),
    InstructionDefinition("ADDI",   InstructionType::I, "r1, i, r2"),
    InstructionDefinition("JUMP",   InstructionType::J, "j"),
    InstructionDefinition("BRZ",    InstructionType::I, "r1, j"),
    InstructionDefinition("ADD4",   InstructionType::R, "r1, r2, r3, r4, r5")
});

void TestExecutionEngine::micro_jump_eq_uses_alu_result()
{
    Machine::MachineState state(Machine::MachineConfig{});
    state.setA(10);
    state.setB(10);

    Microcode::Instruction mi;
    mi.alu = "SUB";   // A - B = 0
    mi.s1 = "A";
    mi.s2 = "B";
    mi.jcond = "EQ";
    mi.adr = "";      // sequential jump

    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.instructions.push_back(mi);

    ExecutionEngine eng(state, mc, {}, testInstructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(eng.currentMicroAddress(), 1u);
}

