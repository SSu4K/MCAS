#include "memorymodel.h"

const static QString MEMORRY_HEADER = "[Memory]";
const static QChar HEADER_PREFIX = '[';
const static QChar COMMENT_PREFIX = ';';

const static qsizetype MEMORY_SLICE_SIZE = 32; // in bytes

using namespace MemoryEditor;

MemoryModel::MemoryModel(QObject* parent) : QAbstractTableModel(parent), m_memory(MEMORY_SIZE, 0) {
}

QVariant MemoryModel::headerData(int section, Qt::Orientation orientation, int role) const {
    if (role != Qt::DisplayRole)
        return {};

    if (orientation == Qt::Vertical) {
        int addr = section * m_cols * (int)unitSize;
        return QString("0x%1").arg(addr, (int)unitSize, 16, QChar('0')).toUpper();
    }

    if (orientation == Qt::Horizontal) {
        return QString("+%1").arg(section * (int)unitSize, 2, 16, QChar('0')).toUpper();
    }

    return {};
}

void MemoryModel::setMemory(const QByteArray& mem) {
    m_memory = mem;
    emit layoutChanged();
}

int MemoryModel::rowCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return (m_memory.size() / (int)unitSize + m_cols - 1) / m_cols;
}

int MemoryModel::columnCount(const QModelIndex& parent) const {
    Q_UNUSED(parent);
    return m_cols;
}

QVariant MemoryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid()) return {};

    int halfWordIndex = index.row() * m_cols + index.column();
    int byteIndex = halfWordIndex * 2;
    if (byteIndex + 1 >= m_memory.size()) {
        if (role == Qt::DisplayRole) return QString();
        if (role == Qt::EditRole) return QVariant();
        return {};
    }

    quint16 hw = (quint8(m_memory[byteIndex]) << 8) | quint8(m_memory[byteIndex + 1]);

    if (role == Qt::DisplayRole) {
        return QString("%1").arg(hw, 4, 16, QChar('0')).toUpper();
    } else if (role == Qt::EditRole) {
        // return an integer for editing (delegate will prefer numeric for memory)
        return QVariant::fromValue(static_cast<quint32>(hw));
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
    m_memory.fill(0);
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

    int halfWordIndex = index.row() * m_cols + index.column();
    int byteIndex = halfWordIndex * 2;
    if (byteIndex + 1 >= m_memory.size()) return false;

    m_memory[byteIndex] = static_cast<char>((val >> 8) & 0xFF);
    m_memory[byteIndex + 1] = static_cast<char>(val & 0xFF);

    emit dataChanged(index, index, {Qt::DisplayRole, Qt::EditRole});
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

    QByteArray hex = m_memory.toHex();
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

    m_memory = QByteArray::fromHex(hexEncoded);

    endResetModel();
    return true;
}

