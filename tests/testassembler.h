#ifndef TESTASSEMBLER_H
#define TESTASSEMBLER_H

#include <QtTest/QTest>
#include "Assembly/instructiondefinition.h"
#include "Assembly/labeldata.h"

class TestAssembler: public QObject
{
    Q_OBJECT

private:
    Assembly::InstructionSet testInstructionSet;
    Assembly::LabelData testLabelData;

public:
    TestAssembler(QObject* parent = nullptr);

private slots:
    void initTestCase();

    void General_Parse();
    void RType_Parse_Done();
    void RType_Parse_Fail();

    void IType_Parse_Done();
    void IType_Parse_Fail();
    void IType_Jumps();

    void JType_Parse_Done();
    void JType_Parse_Fail();
    void JType_Jumps();

    void cleanupTestCase();
};

#endif // TESTASSEMBLER_H
