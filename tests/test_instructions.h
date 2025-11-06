#ifndef TEST_INSTRUCTIONS_H
#define TEST_INSTRUCTIONS_H

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

#endif // TEST_INSTRUCTIONS_H
