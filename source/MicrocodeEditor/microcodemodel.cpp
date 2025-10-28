#include "microcodemodel.h"
#include "Common/hexint.h"

#include <QFile>
#include <QTextStream>

using namespace MicrocodeEditor;

const static QString MICROCODE_HEADER = "[Microcode]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';
const static QChar DELIMITER = '|';

MicrocodeModel::MicrocodeModel(QObject* parent)
    : TextTableModel(parent)
{
    sectionHeader = MICROCODE_HEADER;
    headerPrefix = HEADER_PREFIX;
    commentPrefix = COMMENT_PREFIX;
    delimiter = DELIMITER;
}

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

void MicrocodeModel::populateFromStringMatrix(const QList<QList<QString>> &rows){
    clear();
    QList<Instruction> instructions;
    for(auto row: rows){
        Instruction instruction;
        for(qsizetype field = InstructionField::address; field < InstructionField::fieldCount; field++){
            instruction.setFieldValue(field, row[field]);
        }
        instructions.append(instruction);
    }
    setInstructions(instructions);
}

