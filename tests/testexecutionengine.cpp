#include "testexecutionengine.h"
#include "Simulation/executionengine.h"
#include "Simulation/effects.h"
#include "Microcode/instruction.h"
#include "Microcode/microcodedata.h"

#include "Common/hexint.h"


using Sim::ExecutionEngine;
using Sim::Effects;
using Sim::RegWrite;

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

void TestExecutionEngine::uar_advances_without_jump()
{
    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    Machine::MachineState state(Machine::MachineConfig{});
    ExecutionEngine eng(state, mc, {}, instructionSet);

    Sim::Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(eng.currentMicroAddress(), 1);

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(eng.currentMicroAddress(), 2);
}

void TestExecutionEngine::true_jump_sets_uar(){
    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.setValue(Microcode::InstructionField::label, 2, "Target");

    mc.setValue(Microcode::InstructionField::jcond, 0, "True");
    mc.setValue(Microcode::InstructionField::adr, 0, "Target");

    Machine::MachineState state(Machine::MachineConfig{});
    ExecutionEngine eng(state, mc, {}, instructionSet);

    Sim::Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(eng.currentMicroAddress(), 2);
}

void TestExecutionEngine::rr_reads_formals()
{
    Machine::MachineState state(Machine::MachineConfig{});

    state.setA(0);
    state.setB(0);

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

    std::vector<RegWrite> expectedRegs = {{Machine::SpecRegIndex::A, 0, 11}, {Machine::SpecRegIndex::B, 0, 22}};
    QCOMPARE(effects.regs, expectedRegs);
}

void TestExecutionEngine::wf_writes_c_into_formal(){
    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.setValue(Microcode::InstructionField::regs, 0, "WF3");

    Machine::MachineState state(Machine::MachineConfig{});
    state.setC(77);
    state.setReg(3, 0);
    state.setIR(Assembly::RType(0x01, {0, 0, 3}).encode());
    ExecutionEngine eng(state, mc, {}, instructionSet);

    Sim::Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(state.getReg(3), 77u);

    std::vector<RegWrite> expectedRegs = {{3, 0, 77u}};
    QCOMPARE(effects.regs, expectedRegs);
}

void TestExecutionEngine::alu_add_sets_C()
{
    Machine::MachineState state(Machine::MachineConfig{});
    state.setA(5);
    state.setB(3);
    state.setC(0);

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

    std::vector<RegWrite> expectedRegs = {{Machine::SpecRegIndex::C, 0, 8}};
    QCOMPARE(effects.regs, expectedRegs);
}

void TestExecutionEngine::extir_byte_sign_extends(){
    Machine::MachineState state(Machine::MachineConfig{});

    state.setIR(Assembly::IType(3, 1, 2, 0xFF).encode());

    Microcode::Instruction mi;
    mi.alu = "ADD";
    mi.s1 = "Const";
    mi.s2 = "IR";
    mi.dest = "C";
    mi.constant = "1";
    mi.constantValue = 1;
    mi.extir = "Byte";

    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.instructions[0] = mi;

    ExecutionEngine eng(state, mc, {}, instructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(state.getC(), 0u);
}

void TestExecutionEngine::memory_write_stores_word()
{
    Machine::MachineState state(Machine::MachineConfig{});
    state.setMAR(4);
    state.setMDR(0xCAFEBABE);

    Microcode::MicrocodeData mc(Microcode::MicrocodeConfig{});
    mc.instructions[0].mem  = "WW";
    mc.instructions[0].madr = "MAR";

    ExecutionEngine eng(state, mc, {}, instructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QCOMPARE(state.loadWord(4), 0xCAFEBABEu);
}


void TestExecutionEngine::basic_fetch()
{
    Machine::MachineState state(Machine::MachineConfig{});

    state.setReg(1, 1);
    state.setReg(2, 2);
    const uint32_t encoded = Assembly::RType(0x01, {1, 2}).encode();

    const uint32_t address = 4;
    state.setPC(address);
    state.storeWord(address, encoded);
    state.setIR(0);

    Microcode::MicrocodeData mc = MicrocodeData::buildMinimalFetchMicrocode(Microcode::MicrocodeConfig{});
    mc.setValue(Microcode::InstructionField::label, 5, "ADD");

    Microcode::JumpTableData jt;
    Microcode::JumpTableEntry entry;
    entry.opcode = "ADD";
    entry.targets = {"ADD"};
    jt.entries.append(entry);
    ExecutionEngine eng(state, mc, jt, instructionSet);

    Effects effects;
    QString err;

    QVERIFY(eng.stepMicro(effects, err));
    QVERIFY(eng.stepMicro(effects, err));

    QCOMPARE(state.getIR(), encoded);
    QCOMPARE(state.getPC(), address + 4);
    QCOMPARE(eng.currentMicroAddress(), 5);

    std::vector<RegWrite> expectedRegs = {
        {Machine::SpecRegIndex::IR, 0, encoded},
        {Machine::SpecRegIndex::PC, address, address+4},
        {Machine::SpecRegIndex::A, 0, 1},
        {Machine::SpecRegIndex::B, 0, 2}
    };
    QCOMPARE(effects.regs, expectedRegs);

}
