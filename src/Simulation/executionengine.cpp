#include "executionengine.h"

#include <QDebug>

using Machine::word;
using Machine::SpecRegIndex;
using Machine::AluFlags;
using Machine::DecodedInstruction;

using Microcode::Instruction;

namespace Sim {

ExecutionEngine::ExecutionEngine(Machine::MachineState &state,
                                 Microcode::MicrocodeData const &microcode,
                                 Microcode::JumpTableData const &jumpTable,
                                 Assembly::InstructionSet const &instructionSet)
    : m_state(state), m_microcode(microcode), m_jumpTable(jumpTable), m_instructionSet(instructionSet)
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
    qsizetype memoryChangeSize = 0;
    if (memOp == "WB") {
        old = m_state.loadByte(addr);
        m_state.storeByte(value, addr);
        memoryChangeSize = 1;
    }
    else if (memOp == "WH") {
        old = m_state.loadHalf(addr);
        m_state.storeHalf(value, addr);
        memoryChangeSize = 2;
    }
    else if (memOp == "WW") {
        old = m_state.loadWord(addr);
        m_state.storeWord(value, addr);
        memoryChangeSize = 4;
    }
    else{
        err = "Unknown write operation " + memOp;
        return false;
    }

    for(uint32_t i=memoryChangeSize-1; i>=memoryChangeSize; i--){
        effects.memWrites.push_back({addr+i, uint8_t(old & 0xFF)});
        old = old >> 8;
    }

    return true;
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

    const auto flags = m_state.getAluFlags();

    if (jcond == "TRUE") { result = true; return true; }
    if (jcond == "EQ")   { result = flags.zero; return true; }
    if (jcond == "NE")   { result = !flags.zero; return true; }
    if (jcond == "LT")   { result = flags.negative; return true; }
    if (jcond == "GE")   { result = !flags.negative; return true; }

    result = false;
    return true;
}

uint32_t ExecutionEngine::resolveJumpTable(const Instruction &mi, QString &err)
{
    int tableIndex = -1;
    if (mi.jcond == "JUMP1") tableIndex = 0;
    else if (mi.jcond == "JUMP2") tableIndex = 1;
    else {
        err = "Invalid jump table selector";
        return m_microAddress + 1;
    }

    const auto &decoded = m_state.getDecoded();
    const auto &mnemonic = m_instructionSet.getDefinition(decoded.opcode)->mnemonic;
    // find the jump label
    QString jumpLabel = "";

    for(auto &entry : m_jumpTable.entries){
        if(entry.opcode == mnemonic){
            jumpLabel = entry.targets[tableIndex];
            break;
        }
    }

    bool ok=false;
    uint32_t target = m_microcode.labelData.getAddress(jumpLabel, &ok);

    if (!ok) {
        err = "Jump table lookup failed";
        return m_microAddress + 1;
    }

    return target;
}

bool ExecutionEngine::decodeInstruction(quint32 raw,
                                        DecodedInstruction &out,
                                        QString &err)
{
    Assembly::JType tempInstruction = Assembly::JType::decode(raw);
    quint8 opcode = tempInstruction.opcode();
    const auto definition = m_instructionSet.getDefinition(opcode);
    if(definition == nullptr){
        return false;
    }
    Assembly::InstructionType type = definition->type;
    out.type = type;

    switch (type) {
    case Assembly::InstructionType::R: {
        Assembly::RType r = Assembly::RType::decode(raw);
        out.formals = r.formals;
        break;
    }
    case Assembly::InstructionType::I: {
        Assembly::IType i = Assembly::IType::decode(raw);
        out.formals = { i.sourceRegister, i.destinationRegister };
        out.immediate = i.immediate;
        break;
    }
    case Assembly::InstructionType::J: {
        Assembly::JType j = Assembly::JType::decode(raw);
        out.immediate = j.immediate;
        break;
    }
    default:
        err = "Unknown instruction type";
        return false;
    }

    return true;
}

void ExecutionEngine::applyDefaultAB(const DecodedInstruction &d)
{
    if (d.formals.size() >= 1) {
        m_state.setA(m_state.getReg(d.formals[0]));
    }
    if (d.formals.size() >= 2) {
        m_state.setB(m_state.getReg(d.formals[1]));
    }

}


bool ExecutionEngine::fetchAndDecode(QString &err)
{
    quint32 pc = m_state.getPC();
    quint32 raw = m_state.loadWord(pc);

    m_state.setIR(raw);
    m_state.setPC(pc + 4);

    DecodedInstruction decoded;
    if (!decodeInstruction(raw, decoded, err)) {
        return false;
    }

    m_state.setDecoded(decoded);
    applyDefaultAB(decoded);

    return true;
}

bool ExecutionEngine::performRegsOp(const QString &regs, Effects &effects, QString &err)
{
    if (regs.isEmpty()) return true;

    const auto &d = m_state.getDecoded();

    if (regs == "RR") {
        applyDefaultAB(d);
        return true;
    }

    if (regs.startsWith("WF")) {
        int idx = regs.mid(2).toInt() - 1;
        if (idx < 0 || idx >= d.formals.size()) {
            err = "WF index out of range";
            return false;
        }

        quint8 reg = d.formals[idx];
        uint32_t old = m_state.getReg(reg);
        uint32_t val = m_state.getC();

        m_state.setReg(reg, val);
        effects.regs.push_back({reg, old, val});
        return true;
    }

    err = "Unsupported Regs op: " + regs;
    return false;
}


void ExecutionEngine::advanceMicroAddress(const Instruction &mi,
                                          bool jumpTaken,
                                          QString &err)
{
    uint32_t next = m_microAddress + 1;

    if (jumpTaken) {
        if (!mi.adr.trimmed().isEmpty()) {
            next = mi.jumpAddress;
        }
        else if (mi.jcond == "JUMP1" || mi.jcond == "JUMP2") {
            next = resolveJumpTable(mi, err);
        }
    }

    m_microAddress = next;
}

bool ExecutionEngine::stepMicro(Effects &effects, QString &err)
{
    if (m_microAddress >= static_cast<uint32_t>(m_microcode.instructions.size())) {
        err = "Microaddress out of range";
        return false;
    }

    const Instruction &mi = m_microcode.instructions[m_microAddress];

    bool ok = true;
    uint32_t s1 = 0;
    uint32_t s2 = 0;

    if (!mi.s1.trimmed().isEmpty()) {
        s1 = resolveSource(mi.s1, ok, err, parseConst(mi.constant, ok));
        if (!ok) return false;
    }

    if (!mi.s2.trimmed().isEmpty()) {
        s2 = resolveSource(mi.s2, ok, err, parseConst(mi.constant, ok));
        if (!ok) return false;
    }

    uint32_t aluResult = 0;
    if (!mi.alu.trimmed().isEmpty()) {
        aluResult = computeAluResult(mi.alu, s1, s2);
        AluFlags flags;
        flags.zero = (aluResult == 0);
        flags.negative = (static_cast<int32_t>(aluResult) < 0);
        m_state.setAluFlags(flags);
    }

    if (!mi.dest.trimmed().isEmpty()) {
        performDestWrite(mi.dest, aluResult, effects);
    }

    if (!performMemoryOp(mi, effects, err)) {
        return false;
    }

    bool jumpTaken = false;
    if (!mi.jcond.trimmed().isEmpty()) {
        if (!evaluateJumpCondition(mi.jcond, jumpTaken)) {
            err = "Invalid jump condition";
            return false;
        }
    }

    advanceMicroAddress(mi, jumpTaken, err);

    return true;
}


} // namespace Sim
