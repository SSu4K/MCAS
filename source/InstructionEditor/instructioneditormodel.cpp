#include "instructioneditormodel.h"

using namespace InstructionEditor;

InstructionEditorModel::InstructionEditorModel(QObject* parent, int maxLines, quint32 baseAddress)
    : QAbstractTableModel(parent),
    m_maxLines(maxLines),
    m_baseAddress(baseAddress)
{}

int InstructionEditorModel::rowCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return m_instructions.size();
}

int InstructionEditorModel::columnCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return 4;
}

QVariant InstructionEditorModel::data(const QModelIndex& index, int role) const  {
    if (!index.isValid() || index.row() >= m_instructions.size())
        return {};

    const auto& entry = m_instructions[index.row()];
    const int lineAddr = m_baseAddress + 4 * index.row();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString("0x%1").arg(lineAddr, 4, 16, QChar('0')).toUpper();
            case 1: return entry.valid ? entry.encoded.toHex(' ').toUpper() : QString();
            case INSTRUCTION_COLUMN_INDEX: return entry.text;
            case 3: return entry.valid ? "OK" : entry.errorMessage;
        }
    }

    if (role == Qt::BackgroundRole) {
        if (!entry.valid)
            return QColor(255, 100, 100, 40);
    }

    if (role == Qt::ToolTipRole) {
        if (!entry.valid)
            return entry.errorMessage;
    }

    return {};
}

QVariant InstructionEditorModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
    case 0: return "Address";
    case 1: return "Encoded";
    case INSTRUCTION_COLUMN_INDEX: return "Instruction";
    case 3: return "Status";
    default: return {};
    }
}

Qt::ItemFlags InstructionEditorModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == INSTRUCTION_COLUMN_INDEX)
        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

bool InstructionEditorModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole || index.column() != INSTRUCTION_COLUMN_INDEX)
        return false;

    auto& entry = m_instructions[index.row()];
    entry.text = value.toString();

    auto lineNumber = index.row();
    auto result = m_parser.parseLine(lineNumber, entry.text);
    entry.valid = result.status.isOk();
    entry.errorMessage = result.status.msg;
    entry.encoded.clear();

    if (entry.valid)
        entry.encoded = 0; // zero for now because no correct object casting present

    emit dataChanged(index, this->index(index.row(), 3)); // update row
    return true;
}

bool InstructionEditorModel::addInstruction(const QString& text) {
    if (m_instructions.size() >= m_maxLines)
        return false;

    beginInsertRows({}, m_instructions.size(), m_instructions.size());
    if(!text.isEmpty()){
        m_instructions.append({0, text});
    }
    else{
        m_instructions.append(InstructionEntry());
    }
    endInsertRows();
    return true;
}

bool InstructionEditorModel::removeInstruction(int row) {
    if (row < 0 || row >= m_instructions.size())
        return false;

    beginRemoveRows({}, row, row);
    m_instructions.removeAt(row);
    endRemoveRows();
    return true;
}

QList<QByteArray> InstructionEditorModel::encodedInstructions() const {
    QList<QByteArray> list;
    for (const auto& e : m_instructions)
        list.append(e.encoded);
    return list;
}

void InstructionEditorModel::setBaseAddress(quint32 addr) {
    if (addr != m_baseAddress) {
        m_baseAddress = addr;
        emit dataChanged(index(0,0), index(m_instructions.size()-1,0));
    }
}

quint32 InstructionEditorModel::baseAddress() const { return m_baseAddress; }

int InstructionEditorModel::maxLines() const { return m_maxLines; }
