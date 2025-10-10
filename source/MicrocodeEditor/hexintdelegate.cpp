#include "hexintdelegate.h"

#include "hexintdelegate.h"
#include "hexint.h"
#include <QLineEdit>
#include <QRegularExpression>
#include <QPainter>
#include <QStyleOptionViewItem>

HexIntDelegate::HexIntDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

QWidget* HexIntDelegate::createEditor(QWidget* parent,
                                      const QStyleOptionViewItem&,
                                      const QModelIndex&) const
{
    auto* line = new QLineEdit(parent);
    line->setPlaceholderText("Enter int or 0xHEX");
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    line->setFont(mono);
    return line;
}

void HexIntDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto* line = qobject_cast<QLineEdit*>(editor);
    if (!line) return;

    line->setText(index.data(Qt::EditRole).toString());
    line->selectAll(); // select everything for quick overwrite
}

void HexIntDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                  const QModelIndex& index) const
{
    auto* line = qobject_cast<QLineEdit*>(editor);
    if (!line)
        return;

    QString text = line->text().trimmed();

    bool ok = false;
    int value = text.toInt(&ok, 10);
    if(!ok){
        value = text.toInt(&ok, 16);
    }

    if (ok){
        text = HexInt::intToString(value);
        model->setData(index, HexInt::intToString(value), Qt::EditRole);
    }
}


void HexIntDelegate::updateEditorGeometry(QWidget* editor,
                                          const QStyleOptionViewItem& option,
                                          const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

void HexIntDelegate::paint(QPainter* painter,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QString value = index.data(Qt::DisplayRole).toString();
    opt.text = value;
    QStyledItemDelegate::paint(painter, opt, index);
}
