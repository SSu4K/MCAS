#ifndef JUMPTABLEMODEL_H
#define JUMPTABLEMODEL_H

#include "modellib_export.h"

#include "Common/texttablemodel.h"

// forward declarations
namespace Microcode {
    class JumpTableEntry;
    class JumpTableData;
}

namespace Models{

    class MODELLIB_API JumpTableModel : public TextTableModel {
        Q_OBJECT
    public:
        explicit JumpTableModel(Microcode::JumpTableData *jumpTableData, QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        void setEntries(const QVector<Microcode::JumpTableEntry>& entries);
        QVector<Microcode::JumpTableEntry> entries() const;
        QStringList headers() const;
        void clear();

    private:
        Microcode::JumpTableData *jumpTableData;

        QStringList m_headers = {"Opcode", "Jump Table 1", "Jump Table 2"};

        void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
        bool isRowEmpty(const qsizetype row) const override;
    };
}


#endif // JUMPTABLEMODEL_H
