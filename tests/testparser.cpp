#include "testparser.h"

using namespace InstructionEditor;

void TestParser::initTestCase() {
    qDebug() << "Starting Instruction Parser tests...";
}

void successParseCase(QString instr, InstructionParser &parser){
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

// void TestParser::ITypeParse(){
//     parseStatusCase(
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
//}

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
