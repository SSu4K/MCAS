#include "testparser.h"
#include "InstructionEditor/instructionparser.h"

using namespace InstructionEditor;

void TestParser::initTestCase() {
    qDebug() << "Starting Instruction Parser tests...";
}

void parseCase(QString instr, QVariant expected){
    InstructionParser parser;
    QVariant result = parser.parse(instr);
    QCOMPARE(expected, result);
}

void TestParser::GeneralParse(){
    parseCase(
        "",
        {"Empty line"}
    );
    parseCase(
        "XYZ",
        {"Unknown instruction"}
    );
    parseCase(
        "NOP",
        {"Done"}
    );
}

void TestParser::RTypeParse(){
    parseCase(
        "ADD R1, R2, R3",
        {"Done"}
    );
    parseCase(
        "ADD R1, R2, 0xABCD",
        {"Expected register token for: r3 instead of: 0xABCD"}
    );
    parseCase(
        "ADD R1, R2, R3, R4",
        {"Too many tokens for the format"}
    );
    parseCase(
        "ADD R1, R2",
        {"Expected a token for: , instead of nothing"}
    );
    parseCase(
        "ADD R1, R2,",
        {"Expected a token for: r3 instead of nothing"}
    );
    parseCase(
        "ADD R1, R2(R3)",
        {"Expected separator token: , instead of: ("}
    );
}


void TestParser::cleanupTestCase() {
    qDebug() << "Finished Instruction Parser tests.";
}

//QTEST_MAIN(TestInstructions)
#include "testparser.moc"
