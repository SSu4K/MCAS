#include "executionengine.h"

#include "executionengine.h"
#include <QDebug>

using Machine::word;
using Microcode::Instruction;

namespace Sim {

ExecutionEngine::ExecutionEngine(Machine::MachineState &state,
                                 Microcode::MicrocodeData const &microcode,
                                 Microcode::JumpTableData const &jumpTable)
    : m_state(state), m_microcode(microcode), m_jumpTable(jumpTable)
{
    m_microAddress = 0;
}

uint32_t ExecutionEngine::currentMicroAddress() const {
    return m_microAddress;
}

void ExecutionEngine::setMicroAddress(uint32_t uar) {
    m_microAddress = uar;
}

uint32_t ExecutionEngine::resolveSource(const QString &src, bool &ok, QString &err, uint32_t constValue) const
{
    ok = true;
    QString s = src.trimmed();
    if (s.isEmpty()) return 0;

    if (s.compare("Const", Qt::CaseInsensitive) == 0) {
        bool cOk = false;
        uint32_t v = parseConst(constValue ? QString::number(constValue) : QString(), cOk);
        if (!cOk) { ok = false; err = "Const parse failed"; return 0; }
        return v;
    }
    if (s.compare("PC", Qt::CaseInsensitive) == 0) {
        return m_state.getPc();
    }
    if (s.compare("MAR", Qt::CaseInsensitive) == 0) {
        return m_state.getMAR();
    }
    if (s.compare("MDR", Qt::CaseInsensitive) == 0) {
        return m_state.getMDR();
    }
    if (s.compare("IR", Qt::CaseInsensitive) == 0) {
        return m_state.getIR();
    }
    if (s.compare("A", Qt::CaseInsensitive) == 0) {
        return m_state.getA();
    }
    if (s.compare("B", Qt::CaseInsensitive) == 0) {
        return m_state.getB();
    }
    if (s.compare("C", Qt::CaseInsensitive) == 0) {
        return m_state.getC();
    }

    // fallback: empty or unknown
    ok = false;
    err = QString("Unknown source: %1").arg(s);
    return 0;
}

uint32_t ExecutionEngine::parseConst(const QString &constStr, bool &ok) const
{
    // to consider change (i guess this should match all other contant inputs in the app e.g. default to hex decoding and do decimal 2c of negatives)
    ok = false;
    QString trimmed = constStr.trimmed();
    if (trimmed.isEmpty()) { ok = false; return 0; }
    bool localOk = false;
    uint32_t v = 0;
    if (trimmed.startsWith("0x", Qt::CaseInsensitive)) {
        v = trimmed.mid(2).toUInt(&localOk, 16);
    } else {
        v = trimmed.toUInt(&localOk, 16);
    }
    ok = localOk;
    return v;
}


uint32_t ExecutionEngine::computeAluResult(const QString &aluOp, uint32_t s1, uint32_t s2) const
{
    QString op = aluOp.trimmed().toUpper();
    if (op == "ADD") return s1 + s2;
    if (op == "SUB") return s1 - s2;
    if (op == "RSUB") return s2 - s1;
    if (op == "AND") return s1 & s2;
    if (op == "OR")  return s1 | s2;
    if (op == "XOR") return s1 ^ s2;
    if (op == "S1") return s1;
    if (op == "S2") return s2;
    if (op == "MUL") return s1 * s2;
    if (op == "DIV") return (s2 == 0) ? 0 : (s1 / s2);

    return 0;
}


} // namespace Sim
