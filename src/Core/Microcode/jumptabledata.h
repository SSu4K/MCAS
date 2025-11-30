#ifndef JUMPTABLEDATA_H
#define JUMPTABLEDATA_H

namespace Microcode{

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
