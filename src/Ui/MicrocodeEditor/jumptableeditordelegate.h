#ifndef JUMPTABLEEDITORDELEGATE_H
#define JUMPTABLEEDITORDELEGATE_H

#include <QStyledItemDelegate>

class JumpTableEditorDelegate : public QStyledItemDelegate
{
public:
    explicit JumpTableEditorDelegate(QObject *parent = nullptr): QStyledItemDelegate(parent){}
    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;

    bool eventFilter(QObject* editor, QEvent* event) override;
private:
     mutable QWidget* m_currentEditor = nullptr;
};

#endif // JUMPTABLEEDITORDELEGATE_H
