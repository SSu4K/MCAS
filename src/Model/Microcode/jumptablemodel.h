#ifndef JUMPTABLEMODEL_H
#define JUMPTABLEMODEL_H

#include "Common/texttablemodel.h"

namespace MicrocodeEditor{
    // forward declarations
    class JumpTableEntry;
    class JumpTableData;

    class JumpTableModel : public TextTableModel {
        Q_OBJECT
    public:
        explicit JumpTableModel(JumpTableData *jumpTableData, QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        void setEntries(const QVector<JumpTableEntry>& entries);
        QVector<JumpTableEntry> entries() const;
        QStringList headers() const;
        void clear();
        bool insertEntry(int row, const JumpTableEntry& entry);
        \
    private:
        JumpTableData *jumpTableData;
        QStringList m_headers = {"Opcode", "Jump Table 1", "Jump Table 2"};
        void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
    };
}


#endif // JUMPTABLEMODEL_H
