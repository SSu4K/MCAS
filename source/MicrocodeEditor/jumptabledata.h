#ifndef JUMPTABLEDATA_H
#define JUMPTABLEDATA_H

#include <QStringList>

namespace MicrocodeEditor{

class JumpTableEntry {
public:
    QString opcode;
    QStringList targets;
    JumpTableEntry();
};

class JumpTableData
{
public:
    QList<JumpTableEntry> entries;
    JumpTableData();
};

}

#endif // JUMPTABLEDATA_H
