#include "test_instructions.h"
using namespace InstructionEditor;

void TestInstructions::initTestCase() {
    qDebug() << "Starting Instruction tests...";
}

void TestInstructions::RTypeEncodeAndDecode() {
    RType instr(0, {1, 2, 3, 4, 5});
    auto encoded = instr.encode();
    RType decoded = RType::decode(encoded);
    QCOMPARE(instr, decoded);
}

void TestInstructions::ITypeEncodeAndDecode() {
    IType instr(0, 0, 1, 10);
    auto encoded = instr.encode();
    IType decoded = IType::decode(encoded);
    QCOMPARE(instr, decoded);
}

void TestInstructions::JTypeEncodeAndDecode() {
    JType instr(0, 128);
    auto encoded = instr.encode();
    JType decoded = JType::decode(encoded);
    QCOMPARE(instr, decoded);
}

void TestInstructions::cleanupTestCase() {
    qDebug() << "Finished tests.";
}

//QTEST_MAIN(TestInstructions)
#include "test_instructions.moc"
