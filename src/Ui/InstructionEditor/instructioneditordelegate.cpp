#include <QLineEdit>

#include "instructioneditordelegate.h"
#include "Instruction/instructionmodel.h"

using namespace Ui;

InstructionEditorDelegate::InstructionEditorDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

QWidget* InstructionEditorDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem& option,
                      const QModelIndex& index) const {
    if (index.column() == Models::INSTRUCTION_COLUMN_INDEX || index.column() == Models::LABEL_COLUMN_INDEX){
        QLineEdit* editor = new QLineEdit(parent);
        return editor;
    }

    return nullptr;
}

void InstructionEditorDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    lineEdit->setText(index.data(Qt::DisplayRole).toString());
}

void InstructionEditorDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                  const QModelIndex& index) const {
    QLineEdit* lineEdit = qobject_cast<QLineEdit*>(editor);
    model->setData(index, lineEdit->text(), Qt::EditRole);
}
