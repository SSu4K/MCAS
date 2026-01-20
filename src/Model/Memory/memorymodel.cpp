#include "memorymodel.h"
#include "Machine/machinestate.h"
#include "Common/hexint.h"

const static QString MEMORRY_HEADER = "[Memory]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';

const static qsizetype MEMORY_SLICE_SIZE = 32; // in bytes

const static qsizetype VERTICAL_HEADER_PRECISION = 3;
const static qsizetype HORIZONTAL_HEADER_PRECISION = 2;

using namespace Models;
using namespace Machine;

MemoryModel::MemoryModel(Machine::MachineState* machineState, QObject* parent) :
    QAbstractTableModel(parent),
    machineState(machineState)

{
    machineState->clearMemory();
    setUnitSize(MemoryUnitSize::Half);
    setColumns(8); // just an arbitrary value to init model
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical) {
        int addr = section * m_cols * (int)unitSize;
        return QString("%1").arg(addr, VERTICAL_HEADER_PRECISION, 16, QChar('0')).toUpper();
    }

    if (orientation == Qt::Horizontal) {
        return QString("+%1").arg(section * (int)unitSize, HORIZONTAL_HEADER_PRECISION, 16, QChar('0')).toUpper();
    }

    return {};
}

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_rows;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_cols;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};

    qsizetype i = index.row() * m_cols + index.column();
    quint32 address = i * (qsizetype)unitSize;
    quint32 val = 0;
    try{
        switch(unitSize){
        case MemoryUnitSize::Byte:
            val = machineState->loadByte(address);
            break;
        case MemoryUnitSize::Half:
            val = machineState->loadHalf(address);
            break;
        case MemoryUnitSize::Word:
            val = machineState->loadWord(address);
            break;
        }
    }
    catch(std::exception e){
        // incorrect memory access
        if (role == Qt::DisplayRole) return QString();
        if (role == Qt::EditRole) return QVariant();
        return {};
    }

    if(role == Qt::DisplayRole)
        //return QString("%1").arg(val, 2*(qsizetype)unitSize, 16, QChar('0')).toUpper();
        return HexInt::intToString(val, false, 2*(qsizetype)unitSize);
    else if(role == Qt::EditRole){
        return val;
    }

    return {};
}

void MemoryModel::setColumns(int cols) {
    cols = qMax(1, cols);
    if (m_cols == cols) return;
    beginResetModel();
    m_cols = cols;
    m_rows = (machineState->getMemorySize() / (int)unitSize + m_cols - 1) / m_cols;
    endResetModel();
}


MemoryUnitSize MemoryModel::getUnitSize(){
    return  unitSize;
}

void MemoryModel::clear(){
    machineState->clearMemory();
}

void MemoryModel::setUnitSize(MemoryUnitSize size){
    beginResetModel();
    unitSize = size;
    endResetModel();
}

bool MemoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    quint32 val = value.toInt();

    qsizetype i = index.row() * m_cols + index.column();
    quint32 address = i * (qsizetype)unitSize;
    try{
        switch(unitSize){
        case MemoryUnitSize::Byte:
            machineState->storeByte(address, val); // implicit truncation of quint32 to byte
            break;
        case MemoryUnitSize::Half:
            machineState->storeHalf(address, val); // implicit truncation of quint32 to half
            break;
        case MemoryUnitSize::Word:
            machineState->storeWord(address, val); // implicit truncation of quint32 to word
            break;
        }
        //emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
        emit memoryRegionChanged(address, address + (qsizetype)unitSize - 1);
        return true;
    }
    catch(std::exception e){
        return false;
    }
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MemoryModel::saveToTextStream(QTextStream &stream){
    if (!MEMORRY_HEADER.isEmpty())
        stream << MEMORRY_HEADER << "\n";

    for(qsizetype i=0; i<machineState->getMemorySize(); i++){
        stream << HexInt::intToString(machineState->loadByte(i), false, 2);
        if(i%MEMORY_SLICE_SIZE == MEMORY_SLICE_SIZE-1) stream << "\n";
    }

    return true;
}

static bool findHeader(QTextStream &stream){
    if(MEMORRY_HEADER.isEmpty()){
        return true;
    }
    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line == MEMORRY_HEADER) {
            return true;
        }
    }
    return false;
}

bool MemoryModel::loadFromTextStream(QTextStream &stream){
    if(!findHeader(stream)){
        return false;
    }

    beginResetModel();
    QByteArray hexEncoded;

    while (!stream.atEnd()) {
        QString line = stream.readLine().trimmed();
        if (line.isEmpty())
            continue;
        if (line.startsWith('['))
            break; // new header
        if (line.startsWith(';'))
            continue;

        hexEncoded.append(QByteArray::fromHex(line.toLatin1()));
    }

    for(qsizetype i=0; i<machineState->getMemorySize(); i++){
        machineState->storeByte(i, hexEncoded[i]);
    }

    endResetModel();
    emit memoryRegionChanged(0, machineState->getMemorySize()-1);

    return true;
}

void MemoryModel::onMemoryRegionChanged(const quint32 startAddress, const quint32 endAddress){
    qsizetype firstIndex = startAddress / (int)unitSize;
    qsizetype lastIndex = endAddress / (int)unitSize;

    QModelIndex first = index(firstIndex / m_rows, firstIndex % m_rows);
    QModelIndex last = index(lastIndex / m_rows, lastIndex % m_rows);

    emit dataChanged(first, last, {Qt::DisplayRole, Qt::EditRole});
}

