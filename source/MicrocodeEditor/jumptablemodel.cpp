#include "jumptablemodel.h"
#include "MicrocodeEditor/microcode.h"

#include <QFile>
#include <QTextStream>

const static QString jumpTablesHeader = "[Jump Tables]";

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

void JumpTableModel::clear(){
    beginResetModel();
    m_jumptable.entries = {};
    endResetModel();
}

bool JumpTableModel::loadFromTextStream(QTextStream& stream, QChar delimiter){

    bool inHeader = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        qDebug("Jump Tables line: %s", line.toStdString().data());
        if(line == jumpTablesHeader){
            inHeader = true;
            break;
        }
    }

    if(!inHeader){
        qDebug("Jump Tables header not found!");
        return false;
    }

    qDebug("Jump Tables header found!");

    clear();

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty())
            continue;

        if (line.startsWith(';'))
            continue;

        if (line.startsWith('['))
            break; // found next header

        QStringList parts = line.split(delimiter, Qt::KeepEmptyParts);
        for (QString& s : parts)
            s = s.trimmed();

        if (parts.size() < columnCount()){
            continue;
        }

        JumpTableEntry entry;
        for(qsizetype i = 0; i < columnCount(); i++){
            if(i==0){
                entry.opcode = parts[i];
            }
            else{
                entry.targets.append(parts[i]);
            }
        }
        m_jumptable.entries.append(entry);
    }
    return true;
}

// This refers to width in characters, nothing related to graphics
QList<qsizetype> JumpTableModel::computeColumnWidths() const{
    QList<qsizetype> result = {};
    for (int col = 0; col < columnCount(); col++) {
        qsizetype currentMax = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString().size();
        for (int row = 0; row < rowCount(); row++) {
            qsizetype cellSize = data(index(row, col), Qt::DisplayRole).toString().size();
            if(cellSize>currentMax){
                currentMax = cellSize;
            }
        }
        result.append(currentMax);
    }

    return result;
}

bool JumpTableModel::saveToTextStream(QTextStream& stream, QChar delimiter){
    auto columnWidths = computeColumnWidths();
    stream << jumpTablesHeader << "\n";
    qsizetype width = 0;
    for (int col = 0; col < columnCount(); ++col) {
        auto columnName = headerData(col, Qt::Horizontal, Qt::DisplayRole).toString();
        if(col == 0){
            columnName = ";" + columnName;
        }
        stream << columnName;

        if (col < columnCount() - 1){
            width = columnWidths[col] - columnName.size() + 2;
            stream << qSetFieldWidth(width) << delimiter << qSetFieldWidth(0);
        }
    }
    stream << "\n";
    for (int row = 0; row < rowCount(); ++row) {
        for (int col = 0; col < columnCount(); ++col) {
            QString cell = data(index(row, col), Qt::DisplayRole).toString();
            stream << cell;
            if (col < columnCount() - 1){
                width = columnWidths[col] - cell.size() + 2;
                stream << qSetFieldWidth(width) << delimiter << qSetFieldWidth(0);
            }
        }
        stream << "\n";
    }

    return true;
}

bool JumpTableModel::loadFromTextFile(const QString& filePath, QChar delimiter)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    return loadFromTextStream(in, delimiter);
}
