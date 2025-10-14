#include "jumptablemodel.h"

using namespace MicrocodeEditor;

JumpTableModel::JumpTableModel(QObject* parent)
    : QAbstractTableModel(parent)
{
}

int JumpTableModel::rowCount(const QModelIndex&) const {
    return m_jumptable.entries.size();
}

int JumpTableModel::columnCount(const QModelIndex&) const {
    return m_headers.size();
}

QVariant JumpTableModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || role != Qt::DisplayRole && role != Qt::EditRole)
        return {};

    const auto& entry = m_jumptable.entries[index.row()];
    if (index.column() == 0)
        return entry.opcode;
    else if (index.column() - 1 < entry.targets.size())
        return entry.targets[index.column() - 1];
    return {};
}

bool JumpTableModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (!index.isValid() || role != Qt::EditRole)
        return false;

    auto& entry = m_jumptable.entries[index.row()];
    const QString text = value.toString();

    if (index.column() == 0)
        entry.opcode = text;
    else {
        int targetIdx = index.column() - 1;
        if (targetIdx >= entry.targets.size())
            entry.targets.resize(targetIdx + 1);
        entry.targets[targetIdx] = text;
    }

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

QVariant JumpTableModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole)
        return (section < m_headers.size()) ? m_headers[section] : QString("Col%1").arg(section);
    return {};
}

Qt::ItemFlags JumpTableModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEditable | Qt::ItemIsEnabled;
}

void JumpTableModel::setEntries(const QVector<JumpTableEntry>& entries, const QStringList& headers) {
    beginResetModel();
    m_jumptable.entries = entries;
    m_headers = headers;
    endResetModel();
}

QVector<JumpTableEntry> JumpTableModel::entries() const {
    return m_jumptable.entries;
}

QStringList JumpTableModel::headers() const {
    return m_headers;
}
