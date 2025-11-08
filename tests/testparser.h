#ifndef TESTPARSER_H
#define TESTPARSER_H

#include <QtTest/QTest>
#include "InstructionEditor/instructionparser.h"

class TestParser: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void GeneralParse();
    void RTypeParse();
    void ITypeParse();
    void JTypeParse();

    void cleanupTestCase();
};

#endif // TESTPARSER_H
