#include "executionengine.h"

#include "executionengine.h"
#include <QDebug>

using Machine::word;
using Machine::SpecRegIndex;
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
        return m_state.getPC();
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

    ok = false;
    err = QString("Unknown source: %1").arg(s);
    return 0;
}

uint32_t ExecutionEngine::parseConst(const QString &constStr, bool &ok) const
{
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

bool ExecutionEngine::performDestWrite(const QString &dest, uint32_t value, Effects &effects)
{
    QString d = dest.trimmed().toUpper();
    if (d == "PC") {
        uint32_t old = m_state.getPC();
        m_state.setPC(value);
        effects.regs.push_back({SpecRegIndex::PC, old, value});
        return true;
    }
    else if (d == "MAR") {
        uint32_t old = m_state.getMAR();
        m_state.setMAR(value);
        effects.regs.push_back({SpecRegIndex::MAR, old, value});
        return true;
    }
    else if (d == "MDR") {
        uint32_t old = m_state.getMDR();
        m_state.setMDR(value);
        effects.regs.push_back({SpecRegIndex::MDR, old, value});
        return true;
    }
    else if (d == "A") {
        uint32_t old = m_state.getA();
        m_state.setA(value);
        effects.regs.push_back({SpecRegIndex::A, old, value});
        return true;
    }
    else if (d == "B") {
        uint32_t old = m_state.getB();
        m_state.setB(value);
        effects.regs.push_back({SpecRegIndex::B, old, value});
        return true;
    }
    else if (d == "C") {
        uint32_t old = m_state.getC();
        m_state.setC(value);
        effects.regs.push_back({SpecRegIndex::C, old, value});
        return true;
    }
    else if (d == "TMP") {
        uint32_t old = m_state.getC();
        m_state.setTemp(value);
        effects.regs.push_back({SpecRegIndex::TEMP, old, value});
        return true;
    }

    // Unknown dest
    return false;
}

bool ExecutionEngine::performMemoryRead(const uint32_t addr, const QString &memOp, const QString &memDest, Effects &effects, QString &err)
{
    uint32_t value = 0;
    if (memOp == "RB") {
        value = m_state.loadByte(addr);
    }
    else if (memOp == "RH") {
        value = m_state.loadHalf(addr);
    }
    else if (memOp == "RW") {
        value = m_state.loadWord(addr);
    }
    else{
        err = "Unknown read operation";
        return false;
    }

    if (memDest == "MDR") {
        uint32_t old = m_state.getMAR();
        m_state.setMDR(value);
        effects.regs.push_back({SpecRegIndex::MDR, old, value});
    } else if (memDest == "PC") {
        uint32_t old = m_state.getPC();
        m_state.setPC(value);
        effects.regs.push_back({SpecRegIndex::PC, old, value});
    } else {
        err = "Unknown memory destination " + memOp;
        return false;
    }

    return true;
}

bool ExecutionEngine::performMemoryWrite(const uint32_t addr, const QString &memOp, Effects &effects, QString &err)
{
    uint32_t value = m_state.getMDR();
    uint32_t old = 0;
    if (memOp == "WB") {
        old = m_state.loadByte(addr);
        m_state.storeByte(value, addr);
    }
    else if (memOp == "WH") {
        old = m_state.loadHalf(addr);
        m_state.storeHalf(value, addr);
    }
    else if (memOp == "WW") {
        old = m_state.loadWord(addr);
        m_state.storeWord(value, addr);
    }
    else{
        err = "Unknown write operation " + memOp;
        return false;
    }
}

bool ExecutionEngine::performMemoryOp(const Microcode::Instruction &mi, Effects &effects, QString &err)
{
    QString memOp = mi.mem.trimmed().toUpper();
    if (memOp.isEmpty()) return true;

    QString memAdr = mi.madr.trimmed().toUpper();

    // Determine address source
    uint32_t addr = 0;
    if (memAdr == "MAR") {
        addr = m_state.getMAR();
    } else if (memAdr == "PC") {
        addr = m_state.getPC();
    } else {
        err = "Unknown memory address source";
        return false;
    }

    if(memOp[0] == 'R'){
        QString memDest = mi.dest.trimmed().toUpper();
        return performMemoryRead(addr, memOp, memDest, effects, err);
    }
    else if(memOp[0] == 'W'){
        return performMemoryWrite(addr, memOp, effects, err);
    }

    err = "Unsupported memory op " + mi.mem;
    return false;
}

bool ExecutionEngine::evaluateJumpCondition(const QString &jcond, bool &result) const
{
    QString j = jcond.trimmed().toUpper();
    if (j == "TRUE") { result = true; return true; }
    if (j == "MBUSY") {
        result = false; return true;
    }

    uint32_t a = m_state.getA();
    uint32_t b = m_state.getB();
    if (j == "EQ") { result = (a == b); return true; }
    if (j == "NE") { result = (a != b); return true; }
    if (j == "LT") { result = (static_cast<int32_t>(a) < static_cast<int32_t>(b)); return true; }
    if (j == "GT") { result = (static_cast<int32_t>(a) > static_cast<int32_t>(b)); return true; }
    if (j == "LE") { result = (static_cast<int32_t>(a) <= static_cast<int32_t>(b)); return true; }
    if (j == "GE") { result = (static_cast<int32_t>(a) >= static_cast<int32_t>(b)); return true; }

    result = false;
    return true;
}

} // namespace Sim
