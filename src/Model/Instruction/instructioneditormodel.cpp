#include <QColor>

#include "instructioneditormodel.h"
#include "instructiondata.h"

#include "Assembler/assemblystatus.h"
#include "Common/hexint.h"
#include "machinestate.h"

using namespace Assembly;

using namespace InstructionEditor;

InstructionEditorModel::InstructionEditorModel(
                        MachineState* machineState,
                        Assembly::LabelData* labelData, Assembly::InstructionSet* instructionSet,
                        InstructionData* instructionData, QObject* parent)
    : QAbstractTableModel(parent),
    machineState(machineState),
    labelData(labelData),
    instructionSet(instructionSet),
    instructionData(instructionData),
    m_assembler(instructionSet, labelData),
    m_disassembler(instructionSet, labelData)
{}

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
    AssemblyStatus status;
    auto result = m_assembler.assembleLine(entry.text, lineNumber, status);
    entry.valid = status.isOk();
    entry.errorMessage = status.msg;
    entry.encoded = 0;

    if (entry.valid){
        switch(result->type()){
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
        entry.encoded = result->encode();
        entry.instruction = result;
    }

    quint32 address = 4*lineNumber;
    machineState->storeWord(address, entry.encoded);
    emit memoryRegionChanged(address, address+3);

    emit dataChanged(index, this->index(index.row(), 3)); // update row
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

void InstructionEditorModel::onMemoryRegionChanged(const quint32 startAddress, const quint32 endAddress){
    qsizetype firstline = startAddress / 4;
    qsizetype lastline = endAddress / 4;

    for (qsizetype i = firstline; i <= lastline; i++){
        AssemblyStatus status = AssemblyStatus::done();
        quint32 instruction = machineState->loadWord(4*i);
        QString disassembled = m_disassembler.disassembleLine(instruction, i, status);

        auto& entry = instructionData->instructions[i];
        if(status.isOk()){
            entry.valid = true;
            entry.encoded = instruction;
            entry.errorMessage = "";
            entry.text = disassembled;
        }
        else{
            entry.valid = false;
            entry.encoded = instruction;
            entry.errorMessage = "Failed to disassemble from memory";
            entry.text = "";
        }
    }

    QModelIndex firstIndex = this->index(firstline, 0);
    QModelIndex lastIndex = this->index(lastline, 3);
    emit dataChanged(firstIndex, lastIndex);
}
