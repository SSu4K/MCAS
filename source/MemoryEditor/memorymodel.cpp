#include "memorymodel.h"

using namespace MemoryEditor;

MemoryModel::MemoryModel(QObject* parent) : QAbstractTableModel(parent), m_memory(MEMORY_SIZE, 0) {
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical) {
        int addr = section * m_cols * (int)unitSize;
        return QString("0x%1").arg(addr, (int)unitSize, 16, QChar('0')).toUpper();
    }

    if (orientation == Qt::Horizontal) {
        return QString("+%1").arg(section * (int)unitSize, 2, 16, QChar('0')).toUpper();
    }

    return {};
}

void MemoryModel::setMemory(const QByteArray& mem) {
    m_memory = mem;
    emit layoutChanged();
}

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return (m_memory.size() / (int)unitSize + m_cols - 1) / m_cols;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_cols;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    int unitIndex = index.row() * m_cols + index.column();
    int byteIndex = unitIndex * (int)unitSize;
    if (byteIndex + 1 >= m_memory.size()) return {};

    uint32_t hw = 0;
    for(qsizetype i=0; i<(int)unitSize; i++){
        hw = hw << 8;
        hw |= uint8_t(m_memory[byteIndex+i]);
    }
    //uint16_t hw = (uint8_t(m_memory[byteIndex]) << 8) | uint8_t(m_memory[byteIndex+1]);

    return QString("%1").arg(hw, 2*(int)unitSize, 16, QChar('0'));
}

void MemoryModel::setColumns(int cols) {
    cols = qMax(1, cols);
    if (m_cols == cols) return;
    beginResetModel();
    m_cols = cols;
    endResetModel();
}


MemoryUnitSize MemoryModel::getUnitSize(){
    return  unitSize;
}

void MemoryModel::setUnitSize(MemoryUnitSize size){
    beginResetModel();
    unitSize = size;
    endResetModel();
}

