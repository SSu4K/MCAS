#ifndef MEMORYMODEL_H
#define MEMORYMODEL_H

#include "machinestate.h"

namespace MemoryEditor {

    class MemoryModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit MemoryModel(MachineState* machineState, QObject* parent = nullptr);

        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;

        QVariant data(const QModelIndex& index, int role) const override;
        void setColumns(int cols);

        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role = Qt::EditRole) override;

        void clear();

        MemoryUnitSize getUnitSize();
        void setUnitSize(MemoryUnitSize size);

        bool loadFromTextStream(QTextStream &stream);
        bool saveToTextStream(QTextStream &stream);

    private:
        MachineState* machineState;

        int m_cols = 8;
        MemoryUnitSize unitSize = MemoryUnitSize::Half;
    };


} // namespace MemoryEditor

#endif // MEMORYMODEL_H
