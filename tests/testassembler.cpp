#include "testassembler.h"

qsizetype TEST_LINE_NUMBER = 15;

InstructionSet TEST_INSTRUCTION_SET(
    {
        { "NOP",    InstructionType::R, ""},
        { "ADD",    InstructionType::R, "r1, r2, r3"},
        { "LDH",    InstructionType::I, "r2, i(r1)"},
        { "ADDI",   InstructionType::I, "r1, i, r2"},
        { "JUMP",   InstructionType::J, "j"},
        { "BRZ",    InstructionType::I, "r1, j"},
        { "ADD4",   InstructionType::R, "r1, r2, r3, r4, r5"}
    });

LabelData TEST_LABEL_DATA;

auto TEST_INSTRUCTION_SET_SPTR = std::make_shared<InstructionSet>(TEST_INSTRUCTION_SET);
auto TEST_LABEL_DATA_SPTR = std::make_shared<LabelData>(TEST_LABEL_DATA);

void TestAssembler::initTestCase() {
    qDebug() << "Starting Instruction assembler tests...";
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
    auto result = assembler.assembleLine(instr, TEST_LINE_NUMBER, status);
    auto instruction_ptr = std::static_pointer_cast<T>(result);

    QCOMPARE(status.msg, expectedStatus.msg);
    QCOMPARE(instruction_ptr->toString(), expectedInstr.toString());
    QCOMPARE(*instruction_ptr, expectedInstr);
}

void TestAssembler::General_Parse(){
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);
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
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);

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
    auto expected = RType(6, {1, 2, 3, 4, 5});
    QCOMPARE(*result_ptr, expected);
}

void TestAssembler::RType_Parse_Fail(){
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);

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
    auto labelDataStpr = std::make_shared<LabelData>();
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);

    labelDataStpr->setLabel("label", 0xFF);

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
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);
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
    auto labelDataStpr = std::make_shared<LabelData>();
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);

    const qint32 modulus = 1 << (encodingConfig->IImmediateSize()-1); // modulus for 2c

    const qint32 minJump = -modulus;
    const qint32 maxJump = 4*((modulus - 1) >> 2);

    const qint32 minLineJump = minJump / 4;
    const quint32 maxLineJump = maxJump / 4;

    const qint32 baseLineNumber = maxLineJump + 0xF000; // just a high value some in the middle of memory
    labelDataStpr->setLabel("label_zero", 4*(baseLineNumber+1));
    labelDataStpr->setLabel("label_plus", 4*(baseLineNumber+2));
    labelDataStpr->setLabel("label_minus", 4*(baseLineNumber));

    labelDataStpr->setLabel("label_max", 4*(baseLineNumber+1+maxLineJump));
    labelDataStpr->setLabel("label_min", 4*(baseLineNumber+1+minLineJump));

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
        IType(5, 1, 0, modulus-4), AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_max",
        IType(5, 1, 0, maxJump), AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );
    assembleresultCase<IType>(
        baseLineNumber, "BRZ R1, label_min",
        IType(5, 1, 0, modulus+minJump), AssemblyStatus::done("Parsed IType instruction"),
        assembler
        );

    labelDataStpr->setLabel("label_gt_max", 4*(baseLineNumber+1+maxLineJump+1));
    labelDataStpr->setLabel("label_gt_min", 4*(baseLineNumber+1+minLineJump-1));

    AssemblyStatusCase(
        "BRZ R1, label_gt_max",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Jump to label: label_gt_max out of range for I-Type encoding", {TokenType::Identifier, "label_gt_max", 8, 2, TEST_LINE_NUMBER}),
        assembler
        );
    AssemblyStatusCase(
        "BRZ R1, label_gt_min",
        AssemblyStatus::fail(ErrorType::InvalidToken, "Jump to label: label_gt_min out of range for I-Type encoding", {TokenType::Identifier, "label_gt_min", 8, 2, TEST_LINE_NUMBER}),
        assembler
        );
}

void TestAssembler::JType_Parse_Done(){
    auto labelDataStpr = std::make_shared<LabelData>();
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, labelDataStpr);
    labelDataStpr->setLabel("label", 0xFF);

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
    Assembler assembler(TEST_INSTRUCTION_SET_SPTR, TEST_LABEL_DATA_SPTR);
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

void TestAssembler::cleanupTestCase() {
    qDebug() << "Finished Instruction assembler tests.";
}

//QTEST_MAIN(TestInstructions)
#include "testassembler.moc"

