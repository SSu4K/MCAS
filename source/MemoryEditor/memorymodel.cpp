#include "memorymodel.h"

using namespace MemoryEditor;

MemoryModel::MemoryModel(QObject* parent) : QAbstractTableModel(parent) {}

void MemoryModel::setMemory(const QByteArray& mem) {
    m_memory = mem;
    emit layoutChanged();
}

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return (m_memory.size() / 2 + m_cols - 1) / m_cols;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_cols + 1;
} // +1 for address

QVariant MemoryModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (index.column() == 0) {
        int addr = index.row() * m_cols * 2;
        return QString("0x%1").arg(addr, 4, 16, QChar('0'));
    }

    int halfWordIndex = index.row() * m_cols + index.column() - 1;
    int byteIndex = halfWordIndex * 2;
    if (byteIndex + 1 >= m_memory.size()) return {};
    uint16_t hw = (uint8_t(m_memory[byteIndex]) << 8) | uint8_t(m_memory[byteIndex+1]);
    return QString("%1").arg(hw, 4, 16, QChar('0'));
}

void MemoryModel::setColumns(int cols) {
    m_cols = cols;
    emit layoutChanged();
}

