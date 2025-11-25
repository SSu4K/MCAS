#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCompleter>
#include <QLineEdit>
#include <QTimer>
#include <QKeyEvent>
#include <QStringListModel>
#include <qabstractitemview.h>
#include <qsortfilterproxymodel.h>
#include "enumdelegate.h"

void EnumDelegate::setItems(const QStringList& items) {
    m_items = items;
}

QWidget* EnumDelegate::createEditor(QWidget* parent,
                                    const QStyleOptionViewItem&,
                                    const QModelIndex&) const
{
    auto* combo = new QComboBox(parent);
    combo->setEditable(true);
    combo->addItems(m_items);
    combo->setInsertPolicy(QComboBox::NoInsert);

    auto* completer = new QCompleter(m_items, combo);
    completer->setCaseSensitivity(Qt::CaseInsensitive);
    completer->setCompletionMode(QCompleter::PopupCompletion);
    completer->setFilterMode(Qt::MatchStartsWith); combo->setCompleter(completer);

    if (auto* line = combo->lineEdit()) {
        line->selectAll();

        QTimer::singleShot(0, line, [combo]{
            combo->completer()->setCompletionPrefix("");
            combo->completer()->complete();
        });
    }
    return combo;
}

void EnumDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto* combo = qobject_cast<QComboBox*>(editor);
    if (!combo)
        return;

    const QString currentText = index.data(Qt::EditRole).toString();
    const int idx = combo->findText(currentText, Qt::MatchFixedString);

    if (idx >= 0)
        combo->setCurrentIndex(idx);
    else
        combo->setEditText(currentText);

    if (QLineEdit* lineEdit = combo->lineEdit())
        lineEdit->selectAll();
}

void EnumDelegate::setModelData(QWidget* editor,
                  QAbstractItemModel* model,
                  const QModelIndex& index) const
{
    auto* combo = qobject_cast<QComboBox*>(editor);
    if (!combo)
        return;

    const QString text = combo->currentText().trimmed();
    if (!m_items.contains(text, Qt::CaseInsensitive)) {
        return;
        // text = m_items.value(0, "");
    }

    model->setData(index, text, Qt::EditRole);
}

void EnumDelegate::updateEditorGeometry(QWidget* editor,
                          const QStyleOptionViewItem& option,
                          const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

void EnumDelegate::paint(QPainter* painter,
           const QStyleOptionViewItem& option,
           const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    opt.text = index.data(Qt::DisplayRole).toString();
    QStyledItemDelegate::paint(painter, opt, index);
}
