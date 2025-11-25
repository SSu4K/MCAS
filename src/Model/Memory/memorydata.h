#ifndef MEMORYDATA_H
#define MEMORYDATA_H

namespace MemoryEditor {
    const qsizetype MEMORY_SIZE = 512;

class MemoryData
{
public:
    MemoryData();
    QByteArray memory;
};

} // namespace MemoryEditor

#endif // MEMORYDATA_H
