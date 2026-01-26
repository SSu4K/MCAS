#ifndef INSTRUCTIONSETMODEL_H
#define INSTRUCTIONSETMODEL_H

#include "Common/texttablemodel.h"
#include "Assembly/instruction.h"

namespace Assembly{
    class InstructionDefinition;
    class InstructionSet;
}

namespace Models {


class InstructionSetModel : public TextTableModel
{
    Q_OBJECT
public:
    enum Column {
        OpcodeColumn = 0,
        MnemonicColumn,
        TypeColumn,
        FormatColumn,
        ColumnCount
    };

    explicit InstructionSetModel(Assembly::InstructionSet* instructionSet,
                                 QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role) const override;
    QVariant headerData(int section, Qt::Orientation orientation, int role) const override;

    Qt::ItemFlags flags(const QModelIndex& index) const override;
    bool setData(const QModelIndex& index, const QVariant& value, int role) override;

    void clear();

signals:
    void instructionSetChanged();
    void instructionSetReset();

private:
    Assembly::InstructionSet* instructionSet;

    void populateFromStringMatrix(const QList<QList<QString>> &rows) override;
    bool isRowEmpty(const qsizetype row) const override;

    static QString typeToString(Assembly::InstructionType type);
    static Assembly::InstructionType stringToType(const QString& str, bool* okptr = nullptr);
};


} // namespace Models

#endif // INSTRUCTIONSETMODEL_H
