#ifndef instructionmodel_H
#define instructionmodel_H

#include "Assembly/assembler.h"
#include "Assembly/disassembler.h"

// forward declare
namespace Machine{ class MachineState;}
namespace InstructionEditor { class InstructionData; }

namespace Models {

    const qsizetype LABEL_COLUMN_INDEX = 2;
    const qsizetype INSTRUCTION_COLUMN_INDEX = 3;

    class InstructionModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit InstructionModel(Machine::MachineState* machineState, Assembly::LabelData* labelData,
                                  Assembly::InstructionSet* instructionSet,
                                  InstructionEditor::InstructionData* instructionData,
                                  QObject* parent = nullptr);

        int rowCount(const QModelIndex& parent = {}) const override;
        int columnCount(const QModelIndex& parent = {}) const override;

        QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

        Qt::ItemFlags flags(const QModelIndex& index) const override;

        bool setData(const QModelIndex& index, const QVariant& value, int role) override;

        QList<quint32> encodedInstructions() const;
        void setBaseAddress(quint32 addr);
        quint32 baseAddress() const;

        int maxLines() const;
        void syncFromMemory();

    signals:
        void memoryRegionChanged(quint32 startAddress, quint32 endAddress);

    public slots:
        void onMemoryRegionChanged(const quint32 startAddress, const quint32 endAddress);

    private:
        Machine::MachineState* machineState;
        Assembly::LabelData* labelData;
        Assembly::InstructionSet* instructionSet;
        InstructionEditor::InstructionData* instructionData;

        Assembly::Assembler m_assembler;
        Assembly::Disassembler m_disassembler;


        void assembleLine(const qsizetype lineNumber);
        bool setInstruction(const qsizetype lineNumber, const QString &text);
        bool setLabel(const qsizetype lineNumber, const QString &text);
    };


} // namespace InstructionEditor

#endif // instructionmodel_H
