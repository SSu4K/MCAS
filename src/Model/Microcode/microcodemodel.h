#ifndef MICROCODEMODEL_H
#define MICROCODEMODEL_H

#include "Common/texttablemodel.h"
#include "microcodedata.h"

namespace MicrocodeEditor{

const QChar MICROCODE_FILE_DELIMITER = '|';

class MicrocodeModel : public TextTableModel {
        Q_OBJECT
    public:
        explicit MicrocodeModel(MicrocodeData* microcodeData, QObject* parent = nullptr);

        void setMicrocode(const QList<Instruction> instructions);
        void setInstructions(const QList<Instruction> &instructions);
        //Microcode* microcode() { return &m_microcode; }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool insertInstruction(int row, const Instruction& instr = Instruction{});
        void clear();


    private:
        MicrocodeData *microcodeData;
        void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
    };

}

#endif // MICROCODEMODEL_H
