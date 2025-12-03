#ifndef EFFECTS_H
#define EFFECTS_H

namespace Sim {

#include <vector>

struct RegWrite {
    size_t index;
    uint32_t oldValue;
    uint32_t newValue;
};

struct MemWrite {
    uint32_t address;
    uint8_t  oldByte;
};

struct Effects {
    uint32_t oldPc = 0;
    uint32_t newPc = 0;
    std::vector<RegWrite> regs;
    std::vector<MemWrite> memWrites;
};

} // namespace Sim

#endif // EFFECTS_H
