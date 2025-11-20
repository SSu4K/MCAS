#include <QtTest/QTest>

#include "testdisassembler.h"
#include "Assembler/assembler.h"
#include "Assembler/disassembler.h"

using namespace Assembly;

static qsizetype TEST_LINE_NUMBER = 15;

static InstructionSet TEST_INSTRUCTION_SET(
    {
        { "NOP",    InstructionType::R, ""},
        { "ADD",    InstructionType::R, "r1, r2, r3"},
        { "LDH",    InstructionType::I, "r2, i(r1)"},
        { "ADDI",   InstructionType::I, "r1, i, r2"},
        { "JUMP",   InstructionType::J, "j"},
        { "BRZ",    InstructionType::I, "r1, j"},
        { "ADD4",   InstructionType::R, "r1, r2, r3, r4, r5"}
    });

static LabelData TEST_LABEL_DATA;

static auto TEST_INSTRUCTION_SET_SPTR = std::make_shared<InstructionSet>(TEST_INSTRUCTION_SET);
static auto TEST_LABEL_DATA_SPTR = std::make_shared<LabelData>(TEST_LABEL_DATA);

void TestDisassembler::initTestCase() {
    qDebug() << "Starting Instruction disassembler tests...";
}

void TestDisassembler::RType_Done() {
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);
    Disassembler disassembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);
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
    auto labelDataStpr = std::make_shared<LabelData>();
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);
    Disassembler disassembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);
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
    labelDataStpr->setLabel("label_zero", 4*(baseLineNumber+1));
    labelDataStpr->setLabel("label_plus", 4*(baseLineNumber+2));
    labelDataStpr->setLabel("label_minus", 4*(baseLineNumber));

    labelDataStpr->setLabel("label_max", 4*(baseLineNumber+1+maxLineJump));
    labelDataStpr->setLabel("label_min", 4*(baseLineNumber+1+minLineJump));

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
    auto labelDataStpr = std::make_shared<LabelData>();
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);
    Disassembler disassembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);
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

    labelDataStpr->setLabel("label_zero", 0);
    labelDataStpr->setLabel("label_plus", 4);
    labelDataStpr->setLabel("label_max", 4*(maxLineNumber));

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
    qDebug() << "Finished Instruction disassembler tests.";
}

#include "testdisassembler.moc"
