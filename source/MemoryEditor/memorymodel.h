#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QObject>
#include <QAbstractTableModel>

const qsizetype MEMORY_SIZE = 512;

namespace MemoryEditor {

    enum class MemoryUnitSize{Byte=1, Half=2, Word=4};

    class MemoryModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit MemoryModel(QObject* parent = nullptr);

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        void setMemory(const QByteArray& mem);
        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role) const override;
        void setColumns(int cols);

        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

        MemoryUnitSize getUnitSize();
        void setUnitSize(MemoryUnitSize size);

    private:
        QByteArray m_memory;
        int m_cols = 8;
        MemoryUnitSize unitSize = MemoryUnitSize::Half;
    };


} // namespace MemoryEditor

#endif // MEMORYMODEL_H
