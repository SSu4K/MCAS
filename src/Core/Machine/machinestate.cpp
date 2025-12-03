#include "machinestate.h"
#include <stdexcept>
#include <cassert>

using namespace Machine;

MachineState::MachineState(const MachineConfig &config)
    : config(config), regs(new word[config.registerCount]), memory(new byte[config.memorySize]){}

MachineState::~MachineState(){
    delete [] regs;
    delete [] memory;
}

MachineConfig MachineState::getConfig() const{
    return config;
}

word MachineState::getPc() const{
    return pc;
}

void MachineState::setPc(const word address){
    assert(address%4 == 0);
    pc = address;
}

word MachineState::getMAR() const { return mar; }
void MachineState::setMAR(const word value) { mar = value; }

word MachineState::getMDR() const { return mdr; }
void MachineState::setMDR(const word value) { mdr = value; }

word MachineState::getIR() const { return ir; }
void MachineState::setIR(const word value) { ir = value; }

word MachineState::getA() const { return regA; }
void MachineState::setA(const word value) { regA = value; }

word MachineState::getB() const { return regB; }
void MachineState::setB(const word value) { regB = value; }

word MachineState::getC() const { return regC; }
void MachineState::setC(const word value) { regC = value; }

word MachineState::getReg(const size_t index) const{
    if (index >= config.registerCount)
        throw std::out_of_range("Register index out of range");
    return regs[index];
}

void MachineState::setReg(const size_t index, const word value){
    if (index >= config.registerCount)
        throw std::out_of_range("Register index out of range");
    regs[index] = value;
}

void MachineState::checkMemoryAccess(const word address, const size_t size) const{
    if(address > config.memorySize - size)
        throw std::out_of_range("Memory access out of range");
}

byte MachineState::loadByte(const word address) const{
    checkMemoryAccess(address, 1);
    return memory[address];
}
void MachineState::storeByte(const word address, const byte value){
    checkMemoryAccess(address, 1);
    memory[address] = value;
}

half MachineState::loadHalf(const word address) const{
    checkMemoryAccess(address, 2);
    return (memory[address] << 8) | memory[address+1];
}

void MachineState::storeHalf(const word address, const half value){
    checkMemoryAccess(address, 2);

    memory[address] = (value >> 8) & 0xFF;
    memory[address+1] = value & 0xFF;
}

word MachineState::loadWord(const word address) const{
    checkMemoryAccess(address, 4);

    return (memory[address]     << 24) |
           (memory[address+1]   << 16) |
           (memory[address+2]   << 8) |
           memory[address+3];
}

void MachineState::storeWord(const word address, const word value){
    checkMemoryAccess(address, 4);
    memory[address]     = (value >> 24) & 0xFF;
    memory[address+1]   = (value >> 16) & 0xFF;
    memory[address+2]   = (value >> 8) & 0xFF;
    memory[address+3]   = value & 0xFF;
}

word MachineState::getMemorySize() const{
    return config.memorySize;
}

word MachineState::getInstructionMemorySize() const{
    return config.instructionMemorySize;
}

void MachineState::clearInstructionMemory(){
    for(size_t i=0; i<config.instructionMemorySize; i++){
        memory[i] = 0;
    }
}

void MachineState::clearDataMemory(){
    for(size_t i=config.instructionMemorySize; i<config.memorySize; i++){
        memory[i] = 0;
    }
}

void MachineState::clearMemory(){
    for(size_t i=0; i<config.memorySize; i++){
        memory[i] = 0;
    }
}
