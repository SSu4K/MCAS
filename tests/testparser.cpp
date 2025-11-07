#include "testparser.h"

using namespace InstructionEditor;

void TestParser::initTestCase() {
    qDebug() << "Starting Instruction Parser tests...";
}

void successParseCase(QString instr, InstructionParser &parser){
    ParseResult result = parser.parse(instr);
    QCOMPARE(result.status.severity, ErrorSeverity::Correct);
}

void parseStatusCase(QString instr, ParseStatus expected, InstructionParser &parser){
    ParseResult result = parser.parse(instr);
    if(result.status.severity == ErrorSeverity::Error){
        qDebug() << result.status;
    }
    QCOMPARE(result.status.toString(), expected.toString());
}

// void parseResultCase(QString instr, ParseStatus expectedStatus, InstructionParser &parser){
//     ParseResult result = parser.parse(instr);
//     QCOMPARE(*result.instruction, *expected.instruction);
//     QCOMPARE(result.status.toString(), expected.status.toString());
// }

void TestParser::GeneralParse(){
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
    parseStatusCase(
        "ADD R1, R2, R3", ParseStatus::done("Parsed RType instruction"), parser
    );

    parseStatusCase(
        "ADD R1, R2, 0xABCD",
        ParseStatus::fail("Expected register token for: r3 instead of: 0xABCD [token=0xABCD, index=6 char=12]"),
        parser
    );
    // parseCase(
    //     "ADD R1, R2, R3, R4",
    //     {"Too many tokens for the format"}
    // );
    // parseCase(
    //     "ADD R1, R2",
    //     {"Expected a token for: , instead of nothing"}
    // );
    // parseCase(
    //     "ADD R1, R2,",
    //     {"Expected a token for: r3 instead of nothing"}
    // );
    // parseCase(
    //     "ADD R1, R2(R3)",
    //     {"Expected separator token: , instead of: ("}
    // );
}

// void TestParser::ITypeParse(){
//     parseCase(
//         "LDH R2, 0x0022(R1)",
//         {"Done"}
//     );
//     parseCase(
//         "LDH R2, 0x0022(",
//         {"Expected a token for: r1 instead of nothing"}
//     );
//     parseCase(
//         "LDH R2, 0x0022",
//         {"Expected a token for: ( instead of nothing"}
//     );
//     parseCase(
//         "LDH R2, 0x0022, R1",
//         {"Expected separator token: ( instead of: ,"}
//     );
//     parseCase(
//         "LDH R2, R1, 0x0022",
//         {"Expected hex immediate token for: i instead of: R1"}
//     );
//     parseCase(
//         "LDH R2, 0x0022(R1), R3",
//         {"Too many tokens for the format"}
//     );
//     parseCase(
//         "ADDI R1, 0x0022, R2",
//         {"Done"}
//     );
//     parseCase(
//         "ADDI R1, R2, 0x0022",
//         {"Expected hex immediate token for: i instead of: R2"}
//     );
//     parseCase(
//         "ADDI R1, 0x0022",
//         {"Expected a token for: , instead of nothing"}
//     );
//     parseCase(
//         "ADDI R1, 0x0022,",
//         {"Expected a token for: r2 instead of nothing"}
//     );
//     parseCase(
//         "BRZ R1, loop1",
//         {"Done"}
//     );
//     parseCase(
//         "BRZ R1, 0x00FF",
//         {"Invalid label: 0x00FF"}
//     );
// }

// void TestParser::JTypeParse(){
//     parseCase(
//         "JUMP loop1",
//         {"Done"}
//     );
//     parseCase(
//         "JUMP R3",
//         {"Done"}
//     );
//     parseCase(
//         "JUMP label_two",
//         {"Done"}
//     );
//     parseCase(
//         "JUMP -loop1",
//         {"Invalid label: -loop1"}
//     );
//     parseCase(
//         "JUMP 0x0022",
//         {"Invalid label: 0x0022"}
//     );
//     parseCase(
//         "JUMP",
//         {"Expected a token for: j instead of nothing"}
//     );
//     parseCase(
//         "JUMP loop, R1",
//         {"Too many tokens for the format"}
//     );
// }

void TestParser::cleanupTestCase() {
    qDebug() << "Finished Instruction Parser tests.";
}

//QTEST_MAIN(TestInstructions)
#include "testparser.moc"
