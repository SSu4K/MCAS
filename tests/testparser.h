#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QtTest/QTest>
#include "InstructionEditor/instructionparser.h"

class TestParser: public QObject
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

#endif // TESTPARSER_H
