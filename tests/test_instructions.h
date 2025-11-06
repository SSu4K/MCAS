#ifndef TEST_INSTRUCTIONS_H
#define TEST_INSTRUCTIONS_H

#include <QtTest/QTest>
#include "InstructionEditor/instruction.h"

class TestInstructions : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void RTypeEncodeAndDecode();
    void ITypeEncodeAndDecode();
    void JTypeEncodeAndDecode();
    void cleanupTestCase();
};

#endif // TEST_INSTRUCTIONS_H
