#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include <QObject>
#include <QAbstractTableModel>

const qsizetype MEMORY_SIZE = 512;

namespace MemoryEditor {

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

    private:
        QByteArray m_memory;
        int m_cols = 8;
    };


} // namespace MemoryEditor

#endif // MEMORYMODEL_H
