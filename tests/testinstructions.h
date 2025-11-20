#ifndef TESTINSTRUCTIONS_H
#define TESTINSTRUCTIONS_H

#include <QtTest/QTest>

class TestInstructions : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();

    void RTypeEncodeAndDecode();
    void RTypeEncode();

    void ITypeEncodeAndDecode();
    void ITypeEncode();

    void JTypeEncodeAndDecode();
    void JTypeEncode();

    void cleanupTestCase();
};

#endif // TESTINSTRUCTIONS_H
