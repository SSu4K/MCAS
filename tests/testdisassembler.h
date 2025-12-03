#ifndef TESTDISASSEMBLER_H
#define TESTDISASSEMBLER_H

#include <QtTest/QTest>
#include "Assembly/instructiondefinition.h"
#include "Assembly/labeldata.h"

class TestDisassembler: public QObject
{
    Q_OBJECT

private:
    Assembly::InstructionSet testInstructionSet;
    Assembly::LabelData testLabelData;

public:
    TestDisassembler(QObject* parent = nullptr);

private slots:
    void initTestCase();

    void RType_Done();
    void IType_Done();
    void JType_Done();

    void cleanupTestCase();
};

#endif // TESTDISASSEMBLER_H
