#ifndef INSTRUCTIONEDITORMODEL_H
#define INSTRUCTIONEDITORMODEL_H

#include <QAbstractTableModel>
#include <QColor>
#include "instructionparser.h"

namespace InstructionEditor {

    const qsizetype INSTRUCTION_COLUMN_INDEX = 2;

    struct InstructionEntry {
        QByteArray encoded = 0;
        QString text = "NOP";
        bool valid = true;
        QString errorMessage;
    };

    class InstructionEditorModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit InstructionEditorModel(QObject* parent = nullptr,
                                  int maxLines = 512,
                                  quint32 baseAddress = 0);

        int rowCount(const QModelIndex& parent = {}) const override;
        int columnCount(const QModelIndex& parent = {}) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        bool addInstruction(const QString& text = QString());
        bool removeInstruction(int row);

        QList<QByteArray> encodedInstructions() const;
        void setBaseAddress(quint32 addr);
        quint32 baseAddress() const;

        int maxLines() const;

    private:
        QList<InstructionEntry> m_instructions;
        InstructionParser m_parser;
        const int m_maxLines;
        quint32 m_baseAddress;
    };


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORMODEL_H
