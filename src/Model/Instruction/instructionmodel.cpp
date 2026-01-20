#include <QColor>
#include <QtMinMax>

#include "instructionmodel.h"
#include "instructiondata.h"

#include "Assembly/assemblystatus.h"
#include "Common/labeldata.h"
#include "Common/hexint.h"
#include "Machine/machinestate.h"

using namespace Assembly;
using namespace Models;
using namespace InstructionEditor;

InstructionModel::InstructionModel(
                        Machine::MachineState* machineState,
                        LabelData* labelData, Assembly::InstructionSet* instructionSet,
                        InstructionData* instructionData, QObject* parent)
    : TextTableModel(parent),
    machineState(machineState),
    labelData(labelData),
    instructionSet(instructionSet),
    instructionData(instructionData),
    m_assembler(instructionSet, labelData),
    m_disassembler(instructionSet, labelData)
{}

int InstructionModel::rowCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return instructionData->instructions.size();
}

int InstructionModel::columnCount(const QModelIndex& parent) const  {
    Q_UNUSED(parent);
    return 5;
}

QVariant InstructionModel::data(const QModelIndex& index, int role) const  {
    if (!index.isValid() || index.row() >= instructionData->instructions.size())
        return {};

    const auto& entry = instructionData->instructions[index.row()];
    const int lineAddr = instructionData->baseAddress + 4 * index.row();

    if (role == Qt::DisplayRole) {
        switch (index.column()) {
            case 0: return QString("0x%1").arg(lineAddr, 4, 16, QChar('0')).toUpper();
            case 1: return entry.valid ? HexInt::intToString(entry.encoded, false, 8) : QString();
            case LABEL_COLUMN_INDEX: return entry.label;
            case INSTRUCTION_COLUMN_INDEX: return entry.text;
            case 4: return entry.valid ? "OK" : entry.errorMessage;
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

QVariant InstructionModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole || orientation != Qt::Horizontal)
        return {};

    switch (section) {
        case 0: return "Address";
        case 1: return "Encoded";
        case LABEL_COLUMN_INDEX: return "Label";
        case INSTRUCTION_COLUMN_INDEX: return "Instruction";
        case 4: return "Status";
        default: return {};
    }
}

Qt::ItemFlags InstructionModel::flags(const QModelIndex& index) const {
    if (!index.isValid())
        return Qt::NoItemFlags;

    if (index.column() == INSTRUCTION_COLUMN_INDEX || index.column() == LABEL_COLUMN_INDEX)
        return Qt::ItemIsEditable | Qt::ItemIsSelectable | Qt::ItemIsEnabled;

    return Qt::ItemIsSelectable | Qt::ItemIsEnabled;
}

void InstructionModel::clear(){
    for(qsizetype line=0; line<maxLines(); line++){
        setInstruction(line, "NOP");
        setLabel(line, "");
        assembleLine(line);
    }
}

void InstructionModel::populateFromStringMatrix(const QList<QList<QString>> &rows){
    clear();
    beginResetModel();
    for(qsizetype row=0; row<rows.count(); row++){
        setLabel(row, rows[row][LABEL_COLUMN_INDEX]);
    }
    for(qsizetype row=0; row<rows.count(); row++){
        setInstruction(row, rows[row][INSTRUCTION_COLUMN_INDEX]);
        assembleLine(row);
    }
    endResetModel();
}

void InstructionModel::assembleLine(const qsizetype lineNumber){
    auto& entry = instructionData->instructions[lineNumber];
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

    //emit dataChanged(this->index(lineNumber, 0), this->index(lineNumber, columnCount()-1));
}

bool InstructionModel::setInstruction(const qsizetype lineNumber, const QString &text){
    auto& entry = instructionData->instructions[lineNumber];
    entry.text = text;
    assembleLine(lineNumber);
    return true;
}

bool InstructionModel::setLabel(const qsizetype lineNumber, const QString &text){
    auto& entry = instructionData->instructions[lineNumber];

    if(entry.label.isEmpty() && text.isEmpty()){
        // do nothing
        return true;
    }

    bool ok;
    labelData->getAddress(text, &ok);
    if(ok) return false;

    quint32 address = 4*lineNumber;

    if(entry.label.isEmpty() && !text.isEmpty()){
        // set label
        entry.label = text;
        labelData->setLabel(text, address);
        // reassemble lines that contain new label
        for(qsizetype i=0; i < instructionData->instructions.count(); i++){
            if(instructionData->instructions[i].text.contains(text)){
                assembleLine(i);
            }
        }
    }
    else if(!entry.label.isEmpty() && !text.isEmpty()){
        // change label
        QString oldLabel = entry.label;
        entry.label = text;

        labelData->removeLabel(address);
        labelData->setLabel(text, address);
        // reassemble lines that contain new or old label
        for(qsizetype i=0; i < instructionData->instructions.count(); i++){
            if(instructionData->instructions[i].text.contains(text) || instructionData->instructions[i].text.contains(oldLabel)){
                assembleLine(i);
            }
        }
    }
    else if(!entry.label.isEmpty() && text.isEmpty()){
        // remove label
        QString oldLabel = entry.label;
        entry.label = "";
        labelData->removeLabel(address);
        // reassemble lines that contain old label
        for(qsizetype i=0; i < instructionData->instructions.count(); i++){
            if(instructionData->instructions[i].text.contains(oldLabel)){
                assembleLine(i);
            }
        }
    }

    return true;
}

bool InstructionModel::setData(const QModelIndex& index, const QVariant& value, int role) {
    if (role != Qt::EditRole)
        return false;

    if(index.column() == INSTRUCTION_COLUMN_INDEX){
        return setInstruction(index.row(), value.toString());
    }
    else if(index.column() == LABEL_COLUMN_INDEX){
        return setLabel(index.row(), value.toString());
    }
    return false;
}

QList<quint32> InstructionModel::encodedInstructions() const {
    QList<quint32> list;
    for (const auto& e : instructionData->instructions)
        list.append(e.encoded);
    return list;
}

void InstructionModel::setBaseAddress(quint32 addr) {
    if (addr != instructionData->baseAddress) {
        instructionData->baseAddress = addr;
        emit dataChanged(index(0,0), index(instructionData->instructions.size()-1,0));
    }
}

quint32 InstructionModel::baseAddress() const { return instructionData->baseAddress; }

qsizetype InstructionModel::maxLines() const { return instructionData->maxLines; }

void InstructionModel::onMemoryRegionChanged(const quint32 startAddress, const quint32 endAddress){
    qsizetype firstline = startAddress / 4;
    qsizetype lastline = qMin(endAddress / 4, quint32(maxLines())-1);

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
    QModelIndex lastIndex = this->index(lastline, columnCount()-1);
    emit dataChanged(firstIndex, lastIndex);
}
