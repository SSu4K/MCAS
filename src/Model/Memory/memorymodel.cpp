#include "memorymodel.h"

const static QString MEMORRY_HEADER = "[Memory]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';

const static qsizetype MEMORY_SLICE_SIZE = 32; // in bytes

const static qsizetype VERTICAL_HEADER_PRECISION = 3;
const static qsizetype HORIZONTAL_HEADER_PRECISION = 2;

using namespace MemoryEditor;

MemoryModel::MemoryModel(MemoryData* memoryData, QObject* parent) : QAbstractTableModel(parent), memoryData(memoryData) {
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

void MemoryModel::setMemory(const QByteArray& mem) {
    memoryData->memory = mem;
    emit layoutChanged();
}

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return (memoryData->memory.size() / (int)unitSize + m_cols - 1) / m_cols;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_cols;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};

    qsizetype i = index.row() * m_cols + index.column();
    qsizetype byteIndex = i * (qsizetype)unitSize;
    if (byteIndex + (qsizetype)unitSize > memoryData->memory.size()) {
        if (role == Qt::DisplayRole) return QString();
        if (role == Qt::EditRole) return QVariant();
        return {};
    }

    quint32 val = 0;
    for(qsizetype i=0; i<(int)unitSize; i++){
        val = val << 8;
        val |= quint8(memoryData->memory[byteIndex+i]);
    }

    if (role == Qt::DisplayRole) {
        return QString("%1").arg(val, 2*(qsizetype)unitSize, 16, QChar('0')).toUpper();
    } else if (role == Qt::EditRole) {
        return QVariant::fromValue(static_cast<quint32>(val));
    }

    return {};
}

void MemoryModel::setColumns(int cols) {
    cols = qMax(1, cols);
    if (m_cols == cols) return;
    beginResetModel();
    m_cols = cols;
    endResetModel();
}


MemoryUnitSize MemoryModel::getUnitSize(){
    return  unitSize;
}

void MemoryModel::clear(){
    memoryData->memory.fill(0);
}

void MemoryModel::setUnitSize(MemoryUnitSize size){
    beginResetModel();
    unitSize = size;
    endResetModel();
}

bool MemoryModel::setData(const QModelIndex& index, const QVariant& value, int role)
{
    if (!index.isValid() || role != Qt::EditRole) return false;

    quint32 val = 0;
    bool ok = false;

    if (value.canConvert<quint32>()) {
        val = value.toUInt(&ok);
    } else if (value.typeId() == QMetaType::QString) {
        QString s = value.toString().trimmed();
        if (s.startsWith("0x", Qt::CaseInsensitive)) s.remove(0,2);
        val = s.toUInt(&ok, 16);
        if (!ok) val = s.toUInt(&ok, 10);
    } else {
        val = value.toUInt(&ok);
    }

    if (!ok) return false;

    qsizetype i = index.row() * m_cols + index.column();
    qsizetype byteIndex = i * (qsizetype)unitSize;
    // if (byteIndex + (qsizetype)unitSize > memoryData->memory.size()) return false;

    // for(qsizetype i=(int)unitSize-1; i>=0; i--){
    //     memoryData->memory[byteIndex+i] = val & 0xFF;
    //     val = val >> 8;
    // }
    write(byteIndex, val, unitSize, &ok);
    if (!ok) return false;

    //emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
    return true;
}

Qt::ItemFlags MemoryModel::flags(const QModelIndex& index) const
{
    if (!index.isValid()) return Qt::NoItemFlags;
    return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemIsEditable;
}

bool MemoryModel::saveToTextStream(QTextStream &stream){
    if (!MEMORRY_HEADER.isEmpty())
        stream << MEMORRY_HEADER << "\n";

    QByteArray hex = memoryData->memory.toHex();
    qsizetype slice_size = 2*MEMORY_SLICE_SIZE;
    qsizetype slice_count = hex.size() / slice_size;

    for(qsizetype i=0; i<slice_count; i++){
        auto slice = hex.sliced(i*slice_size, slice_size);
        stream << slice << "\n";
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

        hexEncoded.append(line.toLocal8Bit());
    }

    memoryData->memory = QByteArray::fromHex(hexEncoded);

    endResetModel();
    return true;
}

void MemoryModel::write(const quint32 address, const quint32 value, const MemoryUnitSize unit, bool* okptr){
    qsizetype unitSize = (qsizetype)unit;
    if(address % unitSize != 0 || address + unitSize >= memoryData->memory.size()){
        if(okptr) *okptr = false;
        return;
    }
    quint32 v = value;

    for(qsizetype i=unitSize-1; i>=0; i--){
        memoryData->memory[address+i] = v & 0xFF;
        v = v >> 8;
    }
    if(okptr) *okptr = true;
    emit dataChanged({}, {}, {Qt::DisplayRole, Qt::EditRole});
    return;
}

quint32 MemoryModel::read(quint32 address, MemoryUnitSize unit, bool* okptr){
    qsizetype unitSize = (qsizetype)unit;
    if(address % unitSize != 0 || address + unitSize >= memoryData->memory.size()){
        if(okptr) *okptr = false;
        return 0;
    }
    quint32 result=0;

    for(qsizetype i=0; i<unitSize; i++){
        result = result << 8;
        result |= memoryData->memory[address+i];
    }
    if(okptr) *okptr = true;
    return result;
}

