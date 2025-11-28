#include <QTest>
#include "testmemory.h"
#include "machinestate.h"

TestMemory::TestMemory(QObject *parent)
    : QObject{parent}
{}

void TestMemory::ByteStoreLoad(){
    MachineConfig config;
    MachineState state(config);
    word address;
    byte stored = 0xAB;
    byte loaded;

    address = 0;
    state.storeByte(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, stored);

    address = config.memorySize/2;
    state.storeByte(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, stored);

    address = config.memorySize-1;
    state.storeByte(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, stored);
}

void TestMemory::HalfStoreLoad(){
    MachineConfig config;
    MachineState state(config);
    word address;
    half stored = 0xABCD;
    byte loaded;

    address = 0;
    state.storeHalf(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xCD);

    address = config.memorySize/2;
    state.storeHalf(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xCD);

    address = config.memorySize-2;
    state.storeHalf(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xCD);
}

void TestMemory::WordStoreLoad(){
    MachineConfig config;
    MachineState state(config);
    word address;
    word stored = 0x89ABCDEF;
    byte loaded;

    address = 0;
    state.storeWord(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0x89);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+2);
    QCOMPARE_EQ(loaded, 0xCD);
    loaded = state.loadByte(address+3);
    QCOMPARE_EQ(loaded, 0xEF);


    address = config.memorySize/2;
    state.storeWord(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0x89);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+2);
    QCOMPARE_EQ(loaded, 0xCD);
    loaded = state.loadByte(address+3);
    QCOMPARE_EQ(loaded, 0xEF);

    address = config.memorySize-4;
    state.storeWord(address, stored);
    loaded = state.loadByte(address);
    QCOMPARE_EQ(loaded, 0x89);
    loaded = state.loadByte(address+1);
    QCOMPARE_EQ(loaded, 0xAB);
    loaded = state.loadByte(address+2);
    QCOMPARE_EQ(loaded, 0xCD);
    loaded = state.loadByte(address+3);
    QCOMPARE_EQ(loaded, 0xEF);
}
