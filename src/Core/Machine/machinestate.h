#ifndef MACHINESTATE_H
#define MACHINESTATE_H

#include <stdint.h>

namespace Machine {

typedef uint32_t word;
typedef uint16_t half;
typedef uint8_t byte;

enum class MemoryUnitSize{Byte=1, Half=2, Word=4};

struct MachineConfig{
    word memorySize = 4096;
    word instructionMemorySize = 512;
    byte registerCount = 32;
};

class MachineState
{
private:
    MachineConfig config;
    word pc;
    word *regs;
    byte *memory;

    void checkMemoryAccess(const word address, const size_t size) const;

public:
    explicit MachineState(const MachineConfig &config);
    ~MachineState();

    MachineConfig getConfig() const;
    word getPc() const;
    void setPc(const word address);

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
};

}

#endif // MACHINESTATE_H
