#include "breakpointdelegate.h"

#include <QComboBox>
#include <QLineEdit>
#include <QRegularExpressionValidator>

using namespace Ui;

BreakpointDelegate::BreakpointDelegate(QObject* parent)
    : QStyledItemDelegate(parent)
{
}

QWidget* BreakpointDelegate::createEditor(QWidget* parent,
                                          const QStyleOptionViewItem&,
                                          const QModelIndex& index) const
{
    switch (index.column()) {

    case 0: { // Type
        auto* combo = new QComboBox(parent);
        combo->addItem("PC");
        combo->addItem("uAR");
        return combo;
    }

    case 1: { // Value
        auto* edit = new QLineEdit(parent);
        edit->setPlaceholderText("0x...");
        return edit;
    }

    default:
        return nullptr;
    }
}

void BreakpointDelegate::setEditorData(QWidget* editor,
                                       const QModelIndex& index) const
{
    if (index.column() == 0) {
        auto* combo = qobject_cast<QComboBox*>(editor);
        combo->setCurrentIndex(index.data(Qt::EditRole).toInt());
    }
    else if (index.column() == 1) {
        auto* edit = qobject_cast<QLineEdit*>(editor);
        quint32 value = index.data(Qt::EditRole).toUInt();
        edit->setText(QString("0x%1").arg(value, 8, 16, QChar('0')).toUpper());
    }
}

void BreakpointDelegate::setModelData(QWidget* editor,
                                      QAbstractItemModel* model,
                                      const QModelIndex& index) const
{
    if (index.column() == 0) {
        auto* combo = qobject_cast<QComboBox*>(editor);
        model->setData(index, combo->currentIndex(), Qt::EditRole);
    }
    else if (index.column() == 1) {
        auto* edit = qobject_cast<QLineEdit*>(editor);

        bool ok = false;
        quint32 value = edit->text().toUInt(&ok, 0); // supports 0x...
        if (!ok) value = 0;

        model->setData(index, value, Qt::EditRole);
    }
}
