#include "microcodemodel.h"
#include "hexint.h"

#include <QFile>
#include <QTextStream>

const static QString microcodeHeader = "[Microcode]";

using namespace MicrocodeEditor;

MicrocodeModel::MicrocodeModel(QObject* parent)
    : QAbstractTableModel(parent)
{}

void MicrocodeModel::setMicrocode(Microcode* code) {
    beginResetModel();
    if (code)
        m_microcode = *code;
    else
        m_microcode.instructions.clear();
    endResetModel();
}

void MicrocodeModel::setInstructions(const QList<Instruction> &instructions){
    beginResetModel();
    m_microcode.instructions = instructions;
    endResetModel();
}

int MicrocodeModel::rowCount(const QModelIndex&) const {
    return m_microcode.instructions.size();
}

int MicrocodeModel::columnCount(const QModelIndex&) const {
    return static_cast<int>(InstructionField::fieldCount);
}

QVariant MicrocodeModel::data(const QModelIndex& index, int role) const {
    if (!index.isValid()) return {};

    const auto& instr = m_microcode.instructions[index.row()];

    if (role == Qt::DisplayRole || role == Qt::EditRole) {
        return instr.getFieldValue(index.column());
    }

    return {};
}


bool MicrocodeModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole) return false;
    auto& instr = m_microcode.instructions[index.row()];

    if(!instr.setFieldValue(index.column(), value)){
        return false;
    }

    emit dataChanged(index, index);
    return true;
}

Qt::ItemFlags MicrocodeModel::flags(const QModelIndex& index) const {
    if (!index.isValid()) return Qt::NoItemFlags;

    if(index.column() == InstructionField::address){
        return Qt::ItemIsEnabled | Qt::ItemIsSelectable;
    }

    return Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

QVariant MicrocodeModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return Instruction::fieldNames[section];
    }
    return {};
}

bool MicrocodeModel::insertInstruction(int row, const Instruction& instr) {
    // Clamp row to valid range
    if (row < 0 || row > m_microcode.instructions.size())
        row = m_microcode.instructions.size();

    beginInsertRows(QModelIndex(), row, row);
    m_microcode.instructions.insert(row, instr);
    endInsertRows();

    // Reassign sequential string addresses after insertion
    for (int i = 0; i < m_microcode.instructions.size(); ++i) {
        QString addr = HexInt::intToString(i);
        m_microcode.instructions[i].setFieldValue(InstructionField::address, addr);
    }

    return true;
}

void MicrocodeModel::clear(){
    beginResetModel();
    m_microcode.instructions = {};
    Instruction instr;
    m_microcode.instructions.append(instr);
    endResetModel();
}

// This refers to width in characters, nothing related to graphics
QList<qsizetype> MicrocodeModel::computeColumnWidths() const{
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

bool MicrocodeModel::saveToTextStream(QTextStream& stream, QChar delimiter) const{
    QList<qsizetype> columnWidths = computeColumnWidths();
    stream << microcodeHeader << "\n";
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
bool MicrocodeModel::loadFromTextStream(QTextStream& stream, QChar delimiter){
    QList<Instruction> instructions;

    // Skip all lines until the header
    bool inHeader = false;
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        qDebug("Microcode line: %s", line.toStdString().data());
        if(line == microcodeHeader){
            inHeader = true;
            break;
        }
    }

    if(!inHeader){
        qDebug("Microcode header not found!");
        return false;
    }

    qDebug("Microcode header found!");

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

        Instruction instr;
        for(qsizetype field = InstructionField::address; field < InstructionField::fieldCount; field++){
            instr.setFieldValue(field, parts[field]);
        }
        instructions.append(instr);
    }

    setInstructions(instructions);
    qDebug("Loaded %d microcode lines.", (int)instructions.size());
    return true;
}

bool MicrocodeModel::saveToTextFile(const QString& filePath, QChar delimiter) const
{
    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        return false;

    QTextStream out(&file);
    return saveToTextStream(out, delimiter);
}

bool MicrocodeModel::loadFromTextFile(const QString& filePath, QChar delimiter)
{
    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QTextStream in(&file);
    return loadFromTextStream(in, delimiter);
}

