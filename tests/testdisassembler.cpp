#include <QtTest/QTest>

#include "testdisassembler.h"
#include <QtTest/QTest>
#include "Assembly/assembler.h"
#include "Assembly/disassembler.h"
#include "Common/twoscomplement.h"

#define TEST_LINE_NUMBER 15

using namespace Assembly;

TestDisassembler::TestDisassembler(QObject* parent)
    : QObject(parent),
    testInstructionSet({
        InstructionDefinition("NOP",    InstructionType::R, ""),
        InstructionDefinition("ADD",    InstructionType::R, "r1, r2, r3"),
        InstructionDefinition("LDH",    InstructionType::I, "r2, i(r1)"),
        InstructionDefinition("ADDI",   InstructionType::I, "r1, i, r2"),
        InstructionDefinition("JUMP",   InstructionType::J, "j"),
        InstructionDefinition("BRZ",    InstructionType::I, "r1, j"),
        InstructionDefinition("ADD4",   InstructionType::R, "r1, r2, r3, r4, r5")
    }),
    testLabelData()
{}

void TestDisassembler::initTestCase() {
    qDebug() << "Starting Disassembler tests...";
}

void TestDisassembler::RType_Done() {
    Assembler assembler(&testInstructionSet, &testLabelData);
    Disassembler disassembler(&testInstructionSet, &testLabelData);
    AssemblyStatus status;
    std::shared_ptr<Instruction> instr_ptr = nullptr;
    QString instr_str;
    quint32 encoded;
    QString disassembled;

    instr_str = "ADD R1 R2 R3";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "ADD R31 R17 R5";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "NOP";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "ADD4 R1 R2 R3 R4 R5";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);
}

void TestDisassembler::IType_Done() {
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);
    Disassembler disassembler(&testInstructionSet, &labelData);
    AssemblyStatus status;
    std::shared_ptr<Instruction> instr_ptr = nullptr;
    QString instr_str;
    quint32 encoded;
    QString disassembled;

    instr_str = "LDH R3 0x2222(R8)";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "ADDI R1 0x2222 R3";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 0x2222";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 0xCFF4";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    const qint32 minImmediateValue = TwoC::getMin(encodingConfig->IImmediateSize());
    const qint32 maxImmediateValue = TwoC::getMax(encodingConfig->IImmediateSize());

    const qint32 minLineJump = minImmediateValue / 4;
    const quint32 maxLineJump = maxImmediateValue / 4;

    const qint32 baseLineNumber = maxLineJump + 0xF000; // just a high value some in the middle of memory
    labelData.setLabel("label_zero", 4*(baseLineNumber+1));
    labelData.setLabel("label_plus", 4*(baseLineNumber+2));
    labelData.setLabel("label_minus", 4*(baseLineNumber));

    labelData.setLabel("label_max", 4*(baseLineNumber+1+maxLineJump));
    labelData.setLabel("label_min", 4*(baseLineNumber+1+minLineJump));

    instr_str = "BRZ R1 label_zero";
    instr_ptr = assembler.assembleLine(instr_str, baseLineNumber, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, baseLineNumber, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 label_plus";
    instr_ptr = assembler.assembleLine(instr_str, baseLineNumber, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, baseLineNumber, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 label_minus";
    instr_ptr = assembler.assembleLine(instr_str, baseLineNumber, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, baseLineNumber, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 label_max";
    instr_ptr = assembler.assembleLine(instr_str, baseLineNumber, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, baseLineNumber, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "BRZ R1 label_min";
    instr_ptr = assembler.assembleLine(instr_str, baseLineNumber, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, baseLineNumber, status);

    QCOMPARE(instr_str, disassembled);
}

void TestDisassembler::JType_Done() {
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);
    Disassembler disassembler(&testInstructionSet, &labelData);
    AssemblyStatus status;
    std::shared_ptr<Instruction> instr_ptr = nullptr;
    QString instr_str;
    quint32 encoded;
    QString disassembled;

    instr_str = "JUMP 0x2222";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    const quint32 maxImmediateValue = (1u << encodingConfig->JImmediateSize()) - 1;
    const quint32 maxLineNumber = maxImmediateValue / 4;

    labelData.setLabel("label_zero", 0);
    labelData.setLabel("label_plus", 4);
    labelData.setLabel("label_max", 4*(maxLineNumber));

    instr_str = "JUMP label_zero";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "JUMP label_plus";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);

    instr_str = "JUMP label_max";
    instr_ptr = assembler.assembleLine(instr_str, TEST_LINE_NUMBER, status);
    encoded = instr_ptr->encode();
    disassembled = disassembler.disassembleLine(encoded, TEST_LINE_NUMBER, status);

    QCOMPARE(instr_str, disassembled);
}

void TestDisassembler::cleanupTestCase() {
    qDebug() << "Finished Disassembler tests.";
}

#include "testdisassembler.moc"
