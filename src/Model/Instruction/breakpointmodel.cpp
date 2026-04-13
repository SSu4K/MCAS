#include "breakpointmodel.h"

using namespace Models;
using namespace BreakpointEditor;

const static QString BREAKPOINT_HEADER = "[Breakpoints]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';
const static QChar DELIMITER = '|';

BreakpointModel::BreakpointModel(BreakpointData* data, QObject* parent)
    : TextTableModel(parent),
    m_data(data)
{
    sectionHeader = BREAKPOINT_HEADER;
    headerPrefix = HEADER_PREFIX;
    commentPrefix = COMMENT_PREFIX;
    delimiter = DELIMITER;
}

int BreakpointModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_data->breakpoints.size();
}

int BreakpointModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return 3;
    // 0: Type
    // 1: Value
    // 2: Enabled
}

QVariant BreakpointModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid() || index.row() >= m_data->breakpoints.size())
        return {};

    const auto& bp = m_data->breakpoints[index.row()];

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
        case 0:
            return (bp.type == BreakpointType::PC) ? "PC" : "uAR";
        case 1:
            return QString("0x%1").arg(bp.value, 8, 16, QChar('0')).toUpper();
        case 2:
            return bp.enabled ? "1" : "0";
        }
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case 0: return static_cast<int>(bp.type);
        case 1: return bp.value;
        case 2: return bp.enabled;
        }
    }

    return {};
}

QVariant BreakpointModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
    case 0: return "Type";
    case 1: return "Value";
    case 2: return "Enabled";
    default: return {};
    }
}

Qt::ItemFlags BreakpointModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool BreakpointModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole || !index.isValid())
        return false;

    if (index.row() >= m_data->breakpoints.size())
        return false;

    auto& bp = m_data->breakpoints[index.row()];

    switch (index.column()) {
    case 0: { // Type
        bp.type = static_cast<BreakpointType>(value.toInt());
        break;
    }
    case 1: { // Value
        bool ok = false;
        quint32 val = value.toUInt(&ok);
        if (!ok) return false;
        bp.value = val;
        break;
    }
    case 2: { // Enabled
        bp.enabled = value.toBool();
        break;
    }
    default:
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

bool BreakpointModel::insertRows(int row, int count, const QModelIndex& parent) {
    Q_UNUSED(parent);

    if (count <= 0)
        return false;

    if (m_data->maxBreakpoints > 0 &&
        m_data->breakpoints.size() + count > m_data->maxBreakpoints)
        return false;

    beginInsertRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        BreakpointEntry entry;
        m_data->breakpoints.insert(row, entry);
    }

    endInsertRows();
    return true;
}

bool BreakpointModel::removeRows(int row, int count, const QModelIndex& parent) {
    Q_UNUSED(parent);

    if (count <= 0 || row < 0 || row + count > m_data->breakpoints.size())
        return false;

    beginRemoveRows(QModelIndex(), row, row + count - 1);

    for (int i = 0; i < count; ++i) {
        m_data->breakpoints.removeAt(row);
    }

    endRemoveRows();
    return true;
}

void BreakpointModel::clear() {
    if (m_data->breakpoints.isEmpty())
        return;

    beginResetModel();
    m_data->breakpoints.clear();
    endResetModel();
}

void BreakpointModel::populateFromStringMatrix(const QList<QList<QString>> &rows) {
    beginResetModel();
    m_data->breakpoints.clear();

    for (const auto& row : rows) {
        if (row.size() < 3)
            continue;

        BreakpointEntry entry;

        // Type
        if (row[0] == "PC")
            entry.type = BreakpointType::PC;
        else
            entry.type = BreakpointType::uAR;

        // Value
        bool ok = false;
        entry.value = row[1].toUInt(&ok, 0); // auto-detect base (0x supported)
        if (!ok) entry.value = 0;

        // Enabled
        entry.enabled = (row[2] == "1" || row[2].toLower() == "true");

        m_data->breakpoints.append(entry);
    }

    endResetModel();
}

bool BreakpointModel::isRowEmpty(const qsizetype row) const {
    if (row >= m_data->breakpoints.size())
        return true;

    const auto& bp = m_data->breakpoints[row];

    return (bp.value == 0 && bp.enabled == false);
}
