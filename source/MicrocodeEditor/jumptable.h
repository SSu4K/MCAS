#ifndef JUMPTABLE_H
#define JUMPTABLE_H

#include <QStringList>

namespace MicrocodeEditor{

class JumpTableEntry {
public:
    QString opcode;
    QStringList targets;
    JumpTableEntry();
};

class JumpTable
{
public:
    QList<JumpTableEntry> entries;
    JumpTable();
};

}

#endif // JUMPTABLE_H
