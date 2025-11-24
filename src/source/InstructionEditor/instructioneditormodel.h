#ifndef INSTRUCTIONEDITORMODEL_H
#define INSTRUCTIONEDITORMODEL_H

#include "Assembler/assembler.h"

namespace InstructionEditor {

    // forward declarations
    class InstructionData;

    const qsizetype INSTRUCTION_COLUMN_INDEX = 2;

    class InstructionEditorModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit InstructionEditorModel(QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = {}) const override;
        int columnCount(const QModelIndex& parent = {}) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        bool addInstruction(const QString& text = QString());
        bool removeInstruction(int row);

        QList<quint32> encodedInstructions() const;
        void setBaseAddress(quint32 addr);
        quint32 baseAddress() const;

        int maxLines() const;
        void syncFromMemory();

    private:
        std::shared_ptr<Assembly::LabelData> labelData;
        std::shared_ptr<Assembly::InstructionSet> instructionSet;
        std::shared_ptr<InstructionData> instructionData;
        Assembly::Assembler m_assembler;
    };


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORMODEL_H
