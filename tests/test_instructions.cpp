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

void TestInstructions::RTypeEncode(){
    // Lower bounds for values
    RType instr1(7, {1, 2, 3});
    auto encoded1 = instr1.encode();
    QCOMPARE(encoded1, 0x1C221800);
    // Upper bounds for values
    RType instr2(63, {29, 30, 31});
    auto encoded2 = instr2.encode();
    QCOMPARE(encoded2, 0xFFBEF800);
}

void TestInstructions::ITypeEncodeAndDecode() {
    IType instr(0, 0, 1, 10);
    auto encoded = instr.encode();
    IType decoded = IType::decode(encoded);
    QCOMPARE(instr, decoded);
}

void TestInstructions::ITypeEncode(){
    // Lower bounds for values
    IType instr1(1, 1, 2, 3);
    auto encoded1 = instr1.encode();
    QCOMPARE(encoded1, 0x04220003);
    // Upper bounds for values
    IType instr2(63, 30, 31, 0xFFFF);
    auto encoded2 = instr2.encode();
    QCOMPARE(encoded2, 0xFFDFFFFF);
}

void TestInstructions::JTypeEncodeAndDecode() {
    JType instr(0, 128);
    auto encoded = instr.encode();
    JType decoded = JType::decode(encoded);
    QCOMPARE(instr, decoded);
}

void TestInstructions::JTypeEncode(){
    // Lower bounds for values
    JType instr1(1, 2);
    auto encoded1 = instr1.encode();
    QCOMPARE(encoded1, 0x04000002);
    // Upper bounds for values
    JType instr2(63, 0x3FFFFFF);
    auto encoded2 = instr2.encode();
    QCOMPARE(encoded2, 0xFFFFFFFF);
}

void TestInstructions::cleanupTestCase() {
    qDebug() << "Finished tests.";
}

//QTEST_MAIN(TestInstructions)
#include "test_instructions.moc"
