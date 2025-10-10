#ifndef MICROCODEMODEL_H
#define MICROCODEMODEL_H

#include <QAbstractTableModel>
#include "microcode.h"

namespace MicrocodeEditor{

const QChar MICROCODE_FILE_DELIMITER = '|';

    class MicrocodeModel : public QAbstractTableModel {
        Q_OBJECT
    public:
        explicit MicrocodeModel(QObject* parent = nullptr);

        void setMicrocode(Microcode* code);
        void setInstructions(const QList<Instruction> &instructions);
        Microcode* microcode() { return &m_microcode; }

        int rowCount(const QModelIndex& parent = QModelIndex()) const override;
        int columnCount(const QModelIndex& parent = QModelIndex()) const override;
        QVariant data(const QModelIndex& index, int role) const override;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const override;
        bool setData(const QModelIndex& index, const QVariant& value, int role) override;
        Qt::ItemFlags flags(const QModelIndex& index) const override;
        bool insertInstruction(int row, const Instruction& instr = Instruction{});
        void clear();

        bool saveToTextFile(const QString& filePath, QChar delimiter = MICROCODE_FILE_DELIMITER) const;
        bool loadFromTextFile(const QString& filePath, QChar delimiter = MICROCODE_FILE_DELIMITER);

    private:
        Microcode m_microcode;
    };

}

#endif // MICROCODEMODEL_H
