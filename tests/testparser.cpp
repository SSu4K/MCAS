#include "testparser.h"

using namespace InstructionEditor;

void TestParser::initTestCase() {
    qDebug() << "Starting Instruction Parser tests...";
}

void successparseStatusCase(QString instr, InstructionParser &parser){
    ParseResult result = parser.parseLine(0xF, instr);
    QCOMPARE(result.status.severity, ErrorSeverity::Correct);
}

void parseStatusCase(QString instr, ParseStatus expected, InstructionParser &parser){
    ParseResult result = parser.parseLine(0xF, instr);
    if(result.status.severity == ErrorSeverity::Error){
        qDebug() << result.status;
    }
    QCOMPARE(result.status.toString(), expected.toString());
}

template <class T>
void parseResultCase(const qsizetype lineNumber, const QString &instr, const T &expectedInstr, const ParseStatus &expectedStatus, InstructionParser &parser){
    auto parsed = parser.parseLine(lineNumber, instr);
    auto result_ptr = qSharedPointerCast<T>(parsed.instruction);

    QCOMPARE(parsed.status.msg, expectedStatus.msg);
    QCOMPARE(result_ptr->toString(), expectedInstr.toString());
    QCOMPARE(*result_ptr, expectedInstr);
}

void TestParser::GeneralParse(){
    InstructionParser parser;
    parseStatusCase(
        "", ParseStatus::fail("Empty line"), parser
    );
    parseStatusCase(
        "XYZ", ParseStatus::fail("Unknown instruction"), parser
    );
    parseStatusCase(
        "NOP", ParseStatus::done("Parsed RType instruction"), parser
    );
}

void TestParser::RTypeParse(){
    InstructionParser parser;

    parseStatusCase(
        "ADD R1, R2, R3", ParseStatus::done("Parsed RType instruction"), parser
    );

    if(parser.addInstruction("ADD4", InstructionType::R, "r1, r2, r3, r4, r5")){
        parseStatusCase(
            "ADD4 R1, R2, R3, R4, R5", ParseStatus::done("Parsed RType instruction"), parser
        );

        QString instr = "ADD4 R1, R2, R3, R4, R5";
        auto parsed = parser.parseLine(0xF, instr);
        auto result_ptr = qSharedPointerCast<RType>(parsed.instruction);
        auto expected = RType(6, {1, 2, 3, 4, 5});
        QCOMPARE(*result_ptr, expected);
    }

    parseStatusCase(
        "ADD R1, R2, 0xABCD",
        ParseStatus::fail("Expected register token for: r3 instead of: 0xABCD [token=0xABCD line=15 index=6 char=12]"),
        parser
    );
    parseStatusCase(
        "ADD R1, R2, R3, R4",
        ParseStatus::fail("Too many tokens for the format [token=, line=15 index=7 char=14]"),
        parser
    );
    parseStatusCase(
        "ADD R1, R2",
        ParseStatus::fail("Expected a token for: , instead of nothing"),
        parser
    );
    parseStatusCase(
        "ADD R1, R2,",
        ParseStatus::fail("Expected a token for: r3 instead of nothing"),
        parser
    );
    parseStatusCase(
        "ADD R1, R2(R3)",
        ParseStatus::fail("Expected separator token: , instead of: ( [token=( line=15 index=5 char=10]"),
        parser
    );
}

void ITypeTestJumps(){
    InstructionParser parser;

    const qint32 modulus = 1 << (I_IMMEDIATE_SIZE-1); // modulus for 2c

    const qint32 minJump = -modulus;
    const qint32 maxJump = 4*((modulus - 1) >> 2);

    const qint32 minLineJump = minJump / 4;
    const quint32 maxLineJump = maxJump / 4;

    const qint32 baseLineNumber = maxLineJump + 0xF000; // just a high value some in the middle of memory
    parser.addLabel("label_zero", baseLineNumber+1);
    parser.addLabel("label_plus", baseLineNumber+2);
    parser.addLabel("label_minus", baseLineNumber);

    parser.addLabel("label_max", baseLineNumber+1+maxLineJump);
    parser.addLabel("label_min", baseLineNumber+1+minLineJump);

    parseResultCase<IType>(
        baseLineNumber, "BRZ R1, label_zero",
        IType(5, 1, 0, 0), ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseResultCase<IType>(
        baseLineNumber, "BRZ R1, label_plus",
        IType(5, 1, 0, 4), ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseResultCase<IType>(
        baseLineNumber, "BRZ R1, label_minus",
        IType(5, 1, 0, modulus-4), ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseResultCase<IType>(
        baseLineNumber, "BRZ R1, label_max",
        IType(5, 1, 0, maxJump), ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseResultCase<IType>(
        baseLineNumber, "BRZ R1, label_min",
        IType(5, 1, 0, modulus+minJump), ParseStatus::done("Parsed IType instruction"),
        parser
    );

    parser.addLabel("label_gt_max", baseLineNumber+1+maxLineJump+1);
    parser.addLabel("label_gt_min", baseLineNumber+1+minLineJump-1);

    parseStatusCase(
        "BRZ R1, label_gt_max",
        ParseStatus::fail("Jump to label: label_gt_max out of range for I-Type encoding [token=label_gt_max line=15 index=4 char=8]"),
        parser
    );
    parseStatusCase(
        "BRZ R1, label_gt_min",
        ParseStatus::fail("Jump to label: label_gt_min out of range for I-Type encoding [token=label_gt_min line=15 index=4 char=8]"),
        parser
    );

}

void TestParser::ITypeParse(){
    InstructionParser parser;

    parser.addLabel("label", 0xFF);

    ITypeTestJumps();

    parseStatusCase(
        "LDH R2, 0x0022(R1)",
        ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseStatusCase(
        "ADDI R1, 0x0022, R2",
        ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseStatusCase(
        "ADDI R1, 0x10000, R2",
        ParseStatus::fail("Immediate out of range for 16-bit hex immediate field [token=0x10000 line=15 index=4 char=9]"),
        parser
    );
    parseStatusCase(
        "BRZ R1, label",
        ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseStatusCase(
        "BRZ R1, 0x0008",
        ParseStatus::done("Parsed IType instruction"),
        parser
    );
    parseStatusCase(
        "LDH R2, 0x0022(",
        ParseStatus::fail("Expected a token for: r1 instead of nothing"),
        parser
    );
    parseStatusCase(
        "LDH R2, 0x0022",
        ParseStatus::fail("Expected a token for: ( instead of nothing"),
        parser
    );
    parseStatusCase(
        "LDH R2, 0x0022, R1",
        ParseStatus::fail("Expected separator token: ( instead of: , [token=, line=15 index=5 char=14]"),
        parser
    );
    parseStatusCase(
        "LDH R2, R1, 0x0022",
        ParseStatus::fail("Expected hex immediate token for: i instead of: R1 [token=R1 line=15 index=4 char=8]"),
        parser
    );
    parseStatusCase(
        "LDH R2, 0x0022(R1), R3",
        ParseStatus::fail("Too many tokens for the format [token=, line=15 index=8 char=18]"),
        parser
    );
    parseStatusCase(
        "ADDI R1, R2, 0x0022",
        ParseStatus::fail("Expected hex immediate token for: i instead of: R2 [token=R2 line=15 index=4 char=9]"),
        parser
    );
    parseStatusCase(
        "ADDI R1, 0x0022",
        ParseStatus::fail("Expected a token for: , instead of nothing"),
        parser
    );
    parseStatusCase(
        "ADDI R1, 0x0022,",
        ParseStatus::fail("Expected a token for: r2 instead of nothing"),
        parser
    );
}

void TestParser::JTypeParse(){
    InstructionParser parser;
    parser.addLabel("label", 32);

    parseStatusCase(
        "JUMP 0x0022",
        ParseStatus::done("Parsed JType instruction"),
        parser
    );
    parseStatusCase(
        "JUMP label",
        ParseStatus::done("Parsed JType instruction"),
        parser
    );
    parseStatusCase(
        "JUMP loop1",
        ParseStatus::fail("Unknown label: loop1 [token=loop1 line=15 index=2 char=5]"),
        parser
    );
    parseStatusCase(
        "JUMP",
        ParseStatus::fail("Expected a token for: j instead of nothing"),
        parser
    );
    parseStatusCase(
        "JUMP loop, R1",
        ParseStatus::fail("Too many tokens for the format [token=, line=15 index=3 char=9]"),
        parser
    );
}

void TestParser::cleanupTestCase() {
    qDebug() << "Finished Instruction Parser tests.";
}

//QTEST_MAIN(TestInstructions)
#include "testparser.moc"
