#include "instructioneditormodel.h"
#include "Common/appcontext.h"
#include "Common/hexint.h"

using namespace InstructionEditor;

InstructionEditorModel::InstructionEditorModel(QObject* parent)
    : QAbstractTableModel(parent), m_parser(DEFAULT_INSTRUCTION_SET)
{
    instructionData = AppContext::instance()->sharedData()->instructions().get();
}

int InstructionEditorModel::rowCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return instructionData->instructions.size();
}

int InstructionEditorModel::columnCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return 4;
}

QVariant InstructionEditorModel::data(const QModelIndex& index, int role) const  {
    if (!index.isValid() || index.row() >= instructionData->instructions.size())
        return {};

    const auto& entry = instructionData->instructions[index.row()];
    const int lineAddr = instructionData->baseAddress + 4 * index.row();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString("0x%1").arg(lineAddr, 4, 16, QChar('0')).toUpper();
            case 1: return entry.valid ? HexInt::intToString(entry.encoded, false, 8) : QString();
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

    auto& entry = instructionData->instructions[index.row()];
    entry.text = value.toString();

    auto lineNumber = index.row();
    auto result = m_parser.parseLine(lineNumber, entry.text);
    entry.valid = result.status.isOk();
    entry.errorMessage = result.status.msg;

    if (entry.valid){
        switch(result.instruction->type()){
        case InstructionType::R:
            break;
        case InstructionType::I:
            break;
        case InstructionType::J:
            break;
        default:
            entry.encoded = 0;
            break;
        }
        entry.encoded = result.instruction->encode();
        entry.instruction = result.instruction;

        auto memoryModel = AppContext::instance()->sharedData()->memory().get();
        const qsizetype addr = 4*lineNumber;
        // memoryData->memory[addr + 3] = static_cast<quint8>(entry.encoded & 0xFF);
        // memoryData->memory[addr + 2] = static_cast<quint8>((entry.encoded >> 8) & 0xFF);
        // memoryData->memory[addr + 1] = static_cast<quint8>((entry.encoded >> 16) & 0xFF);
        // memoryData->memory[addr + 0] = static_cast<quint8>((entry.encoded >> 24) & 0xFF);

        memoryModel->write(addr, entry.encoded, MemoryUnitSize::Word);
    }

    emit dataChanged(index, this->index(index.row(), 3)); // update row
    return true;
}

bool InstructionEditorModel::addInstruction(const QString& text) {
    if (instructionData->instructions.size() >= instructionData->maxLines)
        return false;

    beginInsertRows({}, instructionData->instructions.size(), instructionData->instructions.size());
    if(!text.isEmpty()){
        instructionData->instructions.append({0, text});
    }
    else{
        instructionData->instructions.append(InstructionEntry());
    }
    endInsertRows();
    return true;
}

bool InstructionEditorModel::removeInstruction(int row) {
    if (row < 0 || row >= instructionData->instructions.size())
        return false;

    beginRemoveRows({}, row, row);
    instructionData->instructions.removeAt(row);
    endRemoveRows();
    return true;
}

QList<quint32> InstructionEditorModel::encodedInstructions() const {
    QList<quint32> list;
    for (const auto& e : instructionData->instructions)
        list.append(e.encoded);
    return list;
}

void InstructionEditorModel::setBaseAddress(quint32 addr) {
    if (addr != instructionData->baseAddress) {
        instructionData->baseAddress = addr;
        emit dataChanged(index(0,0), index(instructionData->instructions.size()-1,0));
    }
}

quint32 InstructionEditorModel::baseAddress() const { return instructionData->baseAddress; }

int InstructionEditorModel::maxLines() const { return instructionData->maxLines; }

void InstructionEditorModel::syncFromMemory(){
    //
}
