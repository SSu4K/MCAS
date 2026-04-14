#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include "breakpointdata.h"
#include "simLib_export.h"

#include "Machine/machinestate.h"
#include "Microcode/microcodedata.h"
#include "Microcode/jumptabledata.h"
#include "effects.h"
#include "Assembly/instructiondefinition.h"

namespace Sim {

static uint32_t signExtend(uint32_t value, int bits)
{
    const uint32_t fieldMask = (1u << bits) - 1;
    value &= fieldMask;

    const uint32_t signBit = 1u << (bits - 1);
    return (value ^ signBit) - signBit;
}

struct HaltStatus{
    QString reason = "";
    bool isHalted = false;
};

class SIMLIB_API ExecutionEngine
{
public:
    ExecutionEngine(Machine::MachineState &state,
                    Microcode::MicrocodeData const &microcode,
                    Microcode::JumpTableData const &jumpTable,
                    Assembly::InstructionSet const &instructionSet,
                    BreakpointEditor::BreakpointData const &breakpoints

                    );

    ExecutionEngine(Machine::MachineState &state,
                    Microcode::MicrocodeData const &microcode,
                    Microcode::JumpTableData const &jumpTable,
                    Assembly::InstructionSet const &instructionSet
                    );

    uint32_t currentMicroAddress() const;
    void setMicroAddress(uint32_t uar);

    HaltStatus getHaltStatus() const;

    bool stepMicro(Effects &effects, QString &err);
    void unhalt();
    bool reset();

private:
    Machine::MachineState &m_state;
    const Microcode::MicrocodeData &m_microcode;
    const Microcode::JumpTableData &m_jumpTable;
    const Assembly::InstructionSet &m_instructionSet;
    const BreakpointEditor::BreakpointData &m_breakpoints;

    uint32_t m_microAddress = 0;
    HaltStatus haltStatus;

    void halt(const QString &reason);

    uint32_t resolveImmediate(const QString &extir, bool &ok, QString &err) const;
    uint32_t resolveSource(const Microcode::Instruction &mi, const bool &source, bool &ok, QString &err) const;
    uint32_t computeAluResult(const QString &aluOp, uint32_t s1, uint32_t s2) const;
    bool performDestWrite(const QString &dest, uint32_t value, Effects &effects);

    bool performMemoryRead(const uint32_t addr, const QString &memOp, const QString &memDest, Effects &effects, QString &err);
    bool performMemoryWrite(const uint32_t addr, const QString &memOp, Effects &effects, QString &err);
    bool performMemoryOp(const Microcode::Instruction &mi, Effects &effects, QString &err);
    bool evaluateJumpCondition(const QString &jcond, bool &result) const;
    bool performRegsOp(const Microcode::Instruction &mi, Effects &effects, QString &err);
    uint32_t parseConst(const QString &constStr, bool &ok) const;

    uint32_t resolveJumpTable(const Microcode::Instruction &mi, QString &err);

    bool decodeInstruction(quint32 raw, Machine::DecodedInstruction &out, QString &err);

    bool regsRR(Effects &effects);
    bool regsRAF(const qsizetype formalId, Effects &effects);
    bool regsRBF(const qsizetype formalId, Effects &effects);
    bool regsWF(const qsizetype formalId, Effects &effects);

    bool performRegsOp(const QString &regs, Effects &effects, QString &err);

    void advanceMicroAddress(const Microcode::Instruction &mi, bool jumpTaken, QString &err);
};

} // namespace Sim

#endif // EXECUTIONENGINE_H
