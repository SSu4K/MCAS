#ifndef INSTRUCTIONEDITORDELEGATE_H
#define INSTRUCTIONEDITORDELEGATE_H

#include <QStyledItemDelegate>

namespace InstructionEditor {

class InstructionEditorDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit InstructionEditorDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;
    void setEditorData(QWidget* editor, const QModelIndex& index) const override;
    void setModelData(QWidget* editor, QAbstractItemModel* model,
                      const QModelIndex& index) const override;
};


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORDELEGATE_H
