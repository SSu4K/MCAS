#ifndef TESTDISASSEMBLER_H
#define TESTDISASSEMBLER_H

#include <QtTest/QTest>
#include <QObject>

class TestDisassembler: public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void RType_Done();
    void IType_Done();
    void JType_Done();

    void cleanupTestCase();
};

#endif // TESTDISASSEMBLER_H
