#ifndef EXECUTIONENGINE_H
#define EXECUTIONENGINE_H

#include "Machine/machinestate.h"
#include "Microcode/microcodedata.h"
#include "Microcode/jumptabledata.h"
#include "effects.h"
#include "Assembly/instructiondefinition.h"

namespace Sim {

class ExecutionEngine
{
public:
    ExecutionEngine(Machine::MachineState &state,
                    Microcode::MicrocodeData const &microcode,
                    Microcode::JumpTableData const &jumpTable,
                    Assembly::InstructionSet const &instructionSet);

    uint32_t currentMicroAddress() const;
    void setMicroAddress(uint32_t uar);
    bool stepMicro(Effects &effects, QString &err);

private:
    Machine::MachineState &m_state;
    const Microcode::MicrocodeData &m_microcode;
    const Microcode::JumpTableData &m_jumpTable;
    const Assembly::InstructionSet &m_instructionSet;

    uint32_t m_microAddress = 0;

    uint32_t resolveSource(const QString &src, bool &ok, QString &err, uint32_t constValue = 0) const;
    uint32_t computeAluResult(const QString &aluOp, uint32_t s1, uint32_t s2) const;
    bool performDestWrite(const QString &dest, uint32_t value, Effects &effects);

    bool performMemoryRead(const uint32_t addr, const QString &memOp, const QString &memDest, Effects &effects, QString &err);
    bool performMemoryWrite(const uint32_t addr, const QString &memOp, Effects &effects, QString &err);
    bool performMemoryOp(const Microcode::Instruction &mi, Effects &effects, QString &err);
    bool evaluateJumpCondition(const QString &jcond, bool &result) const;
    uint32_t parseConst(const QString &constStr, bool &ok) const;

    uint32_t resolveJumpTable(const Microcode::Instruction &mi, QString &err);

    bool decodeInstruction(quint32 raw, Machine::DecodedInstruction &out, QString &err);
    void applyDefaultAB(const Machine::DecodedInstruction &d);
    bool fetchAndDecode(QString &err);
    bool performRegsOp(const QString &regs, Effects &effects, QString &err);

    void advanceMicroAddress(const Microcode::Instruction &mi, bool jumpTaken, QString &err);
};

} // namespace Sim

#endif // EXECUTIONENGINE_H
