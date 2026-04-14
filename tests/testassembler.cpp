#include <QtTest/QTest>
#include "testassembler.h"
#include "Assembly/assembler.h"
#include "Common/twoscomplement.h"

#define TEST_LINE_NUMBER 15

using namespace Assembly;

TestAssembler::TestAssembler(QObject* parent)
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

void TestAssembler::initTestCase() {
    qDebug() << "Starting Assembler tests...";
}

void successAssemblyStatusCase(QString instr, Assembler &assembler){
    AssemblyStatus status;
    auto result = assembler.assembleLine(instr, TEST_LINE_NUMBER, status);
    QCOMPARE(status.severity, ErrorSeverity::Correct);
}

void AssemblyStatusCase(QString instr, AssemblyStatus expected, Assembler &assembler){
    AssemblyStatus status;
    auto result = assembler.assembleLine(instr, TEST_LINE_NUMBER, status);
    QCOMPARE(status.toString(), expected.toString());
}

template <class T>
void assembleresultCase(const qsizetype lineNumber, const QString &instr, const T &expectedInstr, const AssemblyStatus &expectedStatus, Assembler &assembler){
    AssemblyStatus status;
    auto result = assembler.assembleLine(instr, lineNumber, status);
    auto instruction_ptr = std::static_pointer_cast<T>(result);
    T instruction = *instruction_ptr;

    QCOMPARE(status.msg, expectedStatus.msg);
    QCOMPARE(instruction.toString(), expectedInstr.toString());
    QCOMPARE(instruction, expectedInstr);
}

void TestAssembler::General_Parse(){
    Assembler assembler(&testInstructionSet, &testLabelData);
    AssemblyStatusCase(
        "", AssemblyStatus::fail(ErrorType::MissingToken, "Empty line"), assembler
        );
    AssemblyStatusCase(
        "R1, 0xABCD", AssemblyStatus::fail(ErrorType::WrongToken, "No mnemonic"), assembler
        );
    AssemblyStatusCase(
        "XYZ", AssemblyStatus::fail(ErrorType::InvalidToken, "Unknown instruction"), assembler
        );
    AssemblyStatusCase(
        "NOP", AssemblyStatus::done("Parsed RType instruction"), assembler
        );
}

void TestAssembler::RType_Parse_Done(){
    Assembler assembler(&testInstructionSet, &testLabelData);

    AssemblyStatusCase(
        "ADD R1, R2, R3", AssemblyStatus::done("Parsed RType instruction"), assembler
        );

    AssemblyStatusCase(
        "ADD4 R1, R2, R3, R4, R5", AssemblyStatus::done("Parsed RType instruction"), assembler
        );

    QString instr = "ADD4 R1, R2, R3, R4, R5";
    AssemblyStatus status;
    auto parsed = assembler.assembleLine(instr, TEST_LINE_NUMBER, status);
    auto result_ptr = std::static_pointer_cast<RType>(parsed);
    QCOMPARE_NE(result_ptr, nullptr);
    auto expected = RType(6, {1, 2, 3, 4, 5});

    QCOMPARE(*result_ptr, expected);
}

void TestAssembler::RType_Parse_Fail(){
    Assembler assembler(&testInstructionSet, &testLabelData);

    AssemblyStatusCase(
        "ADD R1, R2, 0xABCD",
        AssemblyStatus::fail(ErrorType::WrongToken, "Expected register token for: r3 instead of: 0xABCD", {TokenType::Hex, "0xABCD", 12, 3, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "ADD R1, R2, R3, R4",
        AssemblyStatus::fail(ErrorType::WrongToken, "Too many tokens for the format", {TokenType::Register, "R4", 16, 4, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "ADD R1, R2",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: r3 instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "ADD R1, R2,",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: r3 instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "ADD R1, R2(R3)",
        AssemblyStatus::fail(ErrorType::WrongToken, "Expected register token for: r3 instead of: (", {TokenType::Bracket, "(", 10, 3, TEST_LINE_NUMBER}),
        assembler
        );
}

void TestAssembler::IType_Parse_Done(){
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);

    labelData.setLabel("label", 0xFF);

    AssemblyStatusCase(
        "LDH R2, 0x0022(R1)",
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    AssemblyStatusCase(
        "ADDI R1, 0x0022, R2",
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    AssemblyStatusCase(
        "BRZ R1, label",
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    AssemblyStatusCase(
        "BRZ R1, 0x0008",
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
}

void TestAssembler::IType_Parse_Fail(){
    Assembler assembler(&testInstructionSet, &testLabelData);
    AssemblyStatusCase(
        "ADDI R1, 0x10000, R2",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Immediate out of range for 16-bit hex immediate field", {TokenType::Hex, "0x10000", 9, 2, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "LDH R2, 0x0022(",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: r1 instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "LDH R2, 0x0022",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: ( instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "LDH R2, 0x0022, R1",
        AssemblyStatus::fail(ErrorType::WrongToken, "Expected bracket token instead of: R1", {TokenType::Register, "R1", 16, 3, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "LDH R2, R1, 0x0022",
        AssemblyStatus::fail(ErrorType::WrongToken, "Expected hex immediate token for: i instead of: R1", {TokenType::Register, "R1", 8, 2, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "LDH R2, 0x0022(R1), R3",
        AssemblyStatus::fail(ErrorType::WrongToken, "Too many tokens for the format", {TokenType::Register, "R3", 20, 6, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "ADDI R1, R2, 0x0022",
        AssemblyStatus::fail(ErrorType::WrongToken, "Expected hex immediate token for: i instead of: R2", {TokenType::Register, "R2", 9, 2, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "ADDI R1, 0x0022",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: r2 instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "ADDI R1, 0x0022,",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: r2 instead of nothing"),
        assembler
        );
}

void TestAssembler::IType_Jumps(){
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);

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

    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_zero",
        IType(5, 1, 0, 0), AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_plus",
        IType(5, 1, 0, 4), AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_minus",
        IType(5, 1, 0, TwoC::toComplement(-4, TwoC::getMin(encodingConfig->IImmediateSize()))),
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );

    // Bacause of alignment the minimum jump is not the minimum value possible to write there,
    // It's the smallest value that fits and is also divisible by 4
    // Therefore here i use 4*minLineJump (necessary) and 4*maxLineJump (just in case)
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_max",
        IType(5, 1, 0, TwoC::toComplement(4*maxLineJump, TwoC::getMin(encodingConfig->IImmediateSize()))),
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_min",
        IType(5, 1, 0, TwoC::toComplement(4*minLineJump, TwoC::getMin(encodingConfig->IImmediateSize()))),
        AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );

    labelData.setLabel("label_gt_max", 4*(baseLineNumber+1+maxLineJump+1));
    labelData.setLabel("label_gt_min", 4*(baseLineNumber+1+minLineJump-1));

    AssemblyStatusCase(
        "BRZ R1, label_gt_max",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Jump to label: label_gt_max out of range for 16-bit encoding", {TokenType::Identifier, "label_gt_max", 8, 2, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "BRZ R1, label_gt_min",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Jump to label: label_gt_min out of range for 16-bit encoding", {TokenType::Identifier, "label_gt_min", 8, 2, TEST_LINE_NUMBER}),
        assembler
        );
}

void TestAssembler::JType_Parse_Done(){
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);

    labelData.setLabel("label", 0xFF);


    AssemblyStatusCase(
        "JUMP 0x0022",
        AssemblyStatus::done("Parsed JType instruction"),
        assembler
        );
    AssemblyStatusCase(
        "JUMP label",
        AssemblyStatus::done("Parsed JType instruction"),
        assembler
        );
}

void TestAssembler::JType_Parse_Fail(){
    Assembler assembler(&testInstructionSet, &testLabelData);
    AssemblyStatusCase(
        "JUMP loop1",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Unknown label: loop1", {TokenType::Identifier, "loop1", 5, 1, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "JUMP",
        AssemblyStatus::fail(ErrorType::MissingToken, "Expected a token for: j instead of nothing"),
        assembler
        );
    AssemblyStatusCase(
        "JUMP loop, R1",
        AssemblyStatus::fail(ErrorType::WrongToken, "Too many tokens for the format", {TokenType::Register, "R1", 11, 2, TEST_LINE_NUMBER}),
        assembler
        );
}

void TestAssembler::JType_Jumps(){
    LabelData labelData;
    Assembler assembler(&testInstructionSet, &labelData);

    labelData.setLabel("label", 0xFF);


    const quint32 maxImmediateValue = (1u << encodingConfig->JImmediateSize()) - 1;
    const quint32 maxLineNumber = maxImmediateValue / 4;

    labelData.setLabel("label_zero", 0);
    labelData.setLabel("label_plus", 4);
    labelData.setLabel("label_max", 4*(maxLineNumber));

    assembleresultCase<JType>(
        TEST_LINE_NUMBER, "JUMP label_zero",
        JType(4, 0), AssemblyStatus::done("Parsed JType instruction"),
        assembler
        );
    assembleresultCase<JType>(
        TEST_LINE_NUMBER, "JUMP label_plus",
        JType(4, 4), AssemblyStatus::done("Parsed JType instruction"),
        assembler
        );

    // Bacause of alignment the minimum jump is not the minimum value possible to write there,
    // It's the smallest value that fits and is also divisible by 4
    // Therefore here i use 4*maxLineNumber (just in case)
    assembleresultCase<JType>(
        TEST_LINE_NUMBER, "JUMP label_max",
        JType(4, 4*maxLineNumber),
        AssemblyStatus::done("Parsed JType instruction"),
        assembler
        );

    labelData.setLabel("label_gt_max", 4*(maxLineNumber+1));

    AssemblyStatusCase(
        "JUMP label_gt_max",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Jump to label: label_gt_max out of range for 26-bit encoding", {TokenType::Identifier, "label_gt_max", 5, 1, TEST_LINE_NUMBER}),
        assembler
        );
}

void TestAssembler::cleanupTestCase() {
    qDebug() << "Finished Assembler tests.";
}

//QTEST_MAIN(TestInstructions)
#include "testassembler.moc"

