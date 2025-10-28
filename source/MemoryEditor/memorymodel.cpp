#include "memorymodel.h"

using namespace MemoryEditor;

MemoryModel::MemoryModel(QObject* parent) : QAbstractTableModel(parent), m_memory(MEMORY_SIZE, 0) {
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical) {
        int addr = section * m_cols * 2;
        return QString("0x%1").arg(addr, 4, 16, QChar('0')).toUpper();
    }

    if (orientation == Qt::Horizontal) {
        // Optional: show offsets for each half-word (00, 02, 04, ...)
        return QString("+%1").arg(section * 2, 2, 16, QChar('0')).toUpper();
    }

    return {};
}

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
    return m_cols;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    int halfWordIndex = index.row() * m_cols + index.column();
    int byteIndex = halfWordIndex * 2;
    if (byteIndex + 1 >= m_memory.size()) return {};
    uint16_t hw = (uint8_t(m_memory[byteIndex]) << 8) | uint8_t(m_memory[byteIndex+1]);
    return QString("%1").arg(hw, 4, 16, QChar('0'));
}

void MemoryModel::setColumns(int cols) {
    cols = qMax(1, cols);
    if (m_cols == cols) return;
    beginResetModel();
    m_cols = cols;
    endResetModel();
}

