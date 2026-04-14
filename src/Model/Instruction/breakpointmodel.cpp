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
    return 4;
    // 0: Index
    // 1: Type
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
            return index.row()+1;
        case 1:
            return (bp.type == BreakpointType::PC) ? "PC" : "uAR";
        case 2:
            return QString("0x%1").arg(bp.value, 8, 16, QChar('0')).toUpper();
        case 3:
            return "";
        }
    }

    if (index.column() == 3 && role == Qt::CheckStateRole) {
        return bp.enabled ? Qt::Checked : Qt::Unchecked;
    }

    if (role == Qt::EditRole) {
        switch (index.column()) {
        case 1: return (bp.type == BreakpointType::PC) ? "PC" : "uAR";
        case 2: return bp.value;
        case 3: return bp.enabled ? Qt::Checked : Qt::Unchecked;;
        }
    }

    return {};
}

QVariant BreakpointModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
    case 0: return "#";
    case 1: return "Type";
    case 2: return "Value";
    case 3: return "Enabled";
    default: return {};
    }
}

Qt::ItemFlags BreakpointModel::flags(const QModelIndex& index) const {
    switch (index.column()) {

    default:
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    case 1: // type
    case 2: // value
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;

    case 3: // enabled checkbox
        return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsUserCheckable;
    }
}

bool BreakpointModel::setData(const QModelIndex& index, const QVariant& value, int role) {

    auto& bp = m_data->breakpoints[index.row()];

    if (index.column() == 3 && role == Qt::CheckStateRole) {
        bp.enabled = (value.toInt() == Qt::Checked);
        emit dataChanged(index, index);
        return true;
    }

    if (role != Qt::EditRole || !index.isValid())
        return false;

    if (index.row() >= m_data->breakpoints.size())
        return false;

    switch (index.column()) {
    case 1: { // Type
        const QString text = value.toString();

        if(!text.compare("PC", Qt::CaseInsensitive)){
            bp.type = BreakpointEditor::BreakpointType::PC;
        }
        else if(!text.compare("uAR", Qt::CaseInsensitive)){
            bp.type = BreakpointEditor::BreakpointType::uAR;
        }
        break;
    }
    case 2: { // Value
        bool ok = false;
        quint32 val = value.toUInt(&ok);
        if (!ok) return false;
        bp.value = val;
        break;
    }
    case 3: { // Enabled
        bp.enabled = (value.toInt() == Qt::Checked);
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
        if (row[1] == "PC")
            entry.type = BreakpointType::PC;
        else
            entry.type = BreakpointType::uAR;

        // Value
        bool ok = false;
        entry.value = row[2].toUInt(&ok, 0); // auto-detect base (0x supported)
        if (!ok) entry.value = 0;

        // Enabled
        entry.enabled = (row[3] == "1" || row[3].toLower() == "true");

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
