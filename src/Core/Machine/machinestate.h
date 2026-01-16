#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <stdint.h>
#include "Assembly/instruction.h"

namespace Machine {

typedef uint32_t word;
typedef uint16_t half;
typedef uint8_t byte;

enum class MemoryUnitSize{Byte=1, Half=2, Word=4};

enum SpecRegIndex{
    PC  = static_cast<size_t>(-1),
    MAR = static_cast<size_t>(-2),
    MDR = static_cast<size_t>(-3),
    IR  = static_cast<size_t>(-4),
    A   = static_cast<size_t>(-5),
    B   = static_cast<size_t>(-6),
    C   = static_cast<size_t>(-7),
    TEMP = static_cast<size_t>(-8)
};

struct DecodedInstruction {
    uint8_t opcode = 0;

    QVector<quint8> formals;
    uint32_t immediateI = 0;
    uint32_t immediateJ = 0;
};

struct MachineConfig{
    word memorySize = 4096;
    word instructionMemorySize = 512;
    byte registerCount = 32;
};

struct AluFlags{
    bool negative = false;
    bool zero = false;
};

class MachineState
{

private:
    MachineConfig config;

    AluFlags aluFlags;

    word pc = 0;
    word mar = 0;
    word mdr = 0;
    word ir  = 0;

    word regA = 0;
    word regB = 0;
    word regC = 0;
    word regTemp = 0;

    DecodedInstruction decoded;

    word *regs;
    byte *memory;

    void checkMemoryAccess(const word address, const size_t size) const;
    bool decodeIR();

public:
    explicit MachineState(const MachineConfig &config);
    ~MachineState();

    MachineConfig getConfig() const;

    AluFlags getAluFlags() const;
    void setAluFlags(const AluFlags &flags);

    word getPC() const;
    void setPC(const word address);

    word getMAR() const;
    void setMAR(const word value);

    word getMDR() const;
    void setMDR(const word value);

    word getIR() const;
    void setIR(const word value);

    word getA() const;
    void setA(const word value);

    word getB() const;
    void setB(const word value);

    word getC() const;
    void setC(const word value);

    word getTemp() const;
    void setTemp(const word value);

    DecodedInstruction getDecoded() const;
    void setDecoded(const DecodedInstruction &decoded);

    word getReg(const size_t index) const;
    void setReg(const size_t index, const word value);

    byte loadByte(const word address) const;
    void storeByte(const word address, const byte value);

    half loadHalf(const word address) const;
    void storeHalf(const word address, const half value);

    word loadWord(const word address) const;
    void storeWord(const word address, const word value);

    word getMemorySize() const;
    word getInstructionMemorySize() const;

    void clearInstructionMemory();
    void clearDataMemory();
    void clearMemory();

    void reset();
};

}

#endif // MACHINESTATE_H
