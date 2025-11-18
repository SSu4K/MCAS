#ifndef TESTASSEMBLER_H
#define TESTASSEMBLER_H

#include <QtTest/QTest>
#include "Assembler/assembler.h"

class TestAssembler: public QObject
{
    Q_OBJECT

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

    void cleanupTestCase();
};

#endif // TESTASSEMBLER_H
