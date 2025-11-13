#include "memorydata.h"

namespace MemoryEditor {

MemoryData::MemoryData() {
    memory = QByteArray(MEMORY_SIZE, 0);
}

} // namespace MemoryEditor
