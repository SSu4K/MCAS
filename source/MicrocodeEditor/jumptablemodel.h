#ifndef JUMPTABLEMODEL_H
#define JUMPTABLEMODEL_H

#include <QAbstractTableModel>
#include <QStringList>
#include <QVector>

#include "jumptable.h"

namespace MicrocodeEditor{
    class JumpTableModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit JumpTableModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;

        void setEntries(const QVector<JumpTableEntry>& entries, const QStringList& headers);
        QVector<JumpTableEntry> entries() const;
        QStringList headers() const;
        void clear();
        bool loadFromTextFile(const QString& filePath, QChar delimiter = '|');

    private:
        JumpTable m_jumptable;
        QStringList m_headers = {"Opcode", "Jump Table 1", "Jump Table 2"};
    };
}


#endif // JUMPTABLEMODEL_H
