#ifndef TESTMEMORY_H
#define TESTMEMORY_H

#include <QObject>

class TestMemory : public QObject
{
    Q_OBJECT
public:
    explicit TestMemory(QObject *parent = nullptr);

private slots:
    void ByteStoreLoad();
    void HalfStoreLoad();
    void WordStoreLoad();
};

#endif // TESTMEMORY_H
