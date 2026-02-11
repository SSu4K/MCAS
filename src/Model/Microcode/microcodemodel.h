#ifndef MICROCODEMODEL_H
#define MICROCODEMODEL_H

#include "modellib_export.h"

#include "Common/texttablemodel.h"
#include "Microcode/microcodedata.h"
#include "Microcode/instruction.h"

namespace Microcode {
    class MicrocodeConfig;
}

namespace Models{

const QChar MICROCODE_FILE_DELIMITER = '|';

class MODELLIB_API MicrocodeModel : public TextTableModel {
        Q_OBJECT
    public:
        explicit MicrocodeModel(Microcode::MicrocodeData* microcodeData, QObject* parent = nullptr);

        void setMicrocode(const QList<Microcode::Instruction> instructions);
        void setInstructions(const QList<Microcode::Instruction> &instructions);
        const Microcode::MicrocodeConfig &microcodeConfig() const { return microcodeData->config; }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool insertInstruction(int row, const Microcode::Instruction& instr = Microcode::Instruction{});
        void clear();

    private:
        Microcode::MicrocodeData *microcodeData;
        void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
        bool isRowEmpty(const qsizetype row) const override;
    };

}

#endif // MICROCODEMODEL_H
