#include "testexecutionengine.h"
#include "Simulation/executionengine.h"
#include "Simulation/effects.h"
#include "Microcode/instruction.h"
#include "Microcode/microcodedata.h"

#include "Common/hexint.h"


using Sim::ExecutionEngine;
using Sim::Effects;

using Microcode::MicrocodeData;

using Assembly::InstructionSet;
using Assembly::InstructionDefinition;
using Assembly::InstructionType;

TestExecutionEngine::TestExecutionEngine(QObject* parent):
    QObject(parent),
    machineConfig(Machine::MachineConfig()),
    instructionSet({
          InstructionDefinition("NOP",    InstructionType::R, ""),
          InstructionDefinition("ADD",    InstructionType::R, "r1, r2, r3"),
          InstructionDefinition("SUB",    InstructionType::R, "r1, r2, r3"),
          InstructionDefinition("LDH",    InstructionType::I, "r2, i(r1)"),
          InstructionDefinition("ADDI",   InstructionType::I, "r1, i, r2"),
          InstructionDefinition("JUMP",   InstructionType::J, "j"),
          InstructionDefinition("BRZ",    InstructionType::I, "r1, j"),
          InstructionDefinition("ADD4",   InstructionType::R, "r1, r2, r3, r4, r5")
      }) {}

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

    ExecutionEngine eng(state, mc, {}, instructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(eng.currentMicroAddress(), 1u);
}

void TestExecutionEngine::micro_alu_sets_result()
{
    Machine::MachineState state(Machine::MachineConfig{});
    state.setA(5);
    state.setB(3);

    Microcode::Instruction mi;
    mi.alu = "ADD";
    mi.s1 = "A";
    mi.s2 = "B";
    mi.dest = "C";

    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.instructions[0] = mi;

    ExecutionEngine eng(state, mc, {}, instructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(state.getC(), 8u);
}

void TestExecutionEngine::fetch_increments_pc_and_sets_ir()
{
    Machine::MachineState state(Machine::MachineConfig{});

    const uint32_t encoded = 0x00BBCCDD;
    const uint32_t address = 4;
    state.setPC(address);
    state.storeWord(address, encoded);

    Microcode::MicrocodeData mc = MicrocodeData::buildMinimalFetchMicrocode(Microcode::MicrocodeConfig{});
    ExecutionEngine eng(state, mc, {}, instructionSet);

    Sim::Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QVERIFY(eng.stepMicro(effects, err));

    QCOMPARE(state.getIR(), encoded);
    QCOMPARE(state.getPC(), address + 4);

}

void TestExecutionEngine::rr_loads_ab_from_formals()
{
    Machine::MachineState state(Machine::MachineConfig{});

    state.setReg(1, 11);
    state.setReg(2, 22);

    quint32 raw = Assembly::RType(0x01, {1, 2, 3}).encode();
    state.setIR(raw);   // decoding happens automatically

    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.instructions[0].regs = "RR";

    ExecutionEngine eng(state, mc, {}, instructionSet);
    Sim::Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));

    QCOMPARE(state.getA(), 11u);
    QCOMPARE(state.getB(), 22u);
}



