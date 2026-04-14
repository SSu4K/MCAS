#ifndef BREAKPOINTDELEGATE_H
#define BREAKPOINTDELEGATE_H

#include <QStyledItemDelegate>

namespace Ui {

class BreakpointDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit BreakpointDelegate(QObject* parent = nullptr);

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem& option,
                          const QModelIndex& index) const override;

    void setEditorData(QWidget* editor,
                       const QModelIndex& index) const override;

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;
};

} // namespace Ui
#endif // BREAKPOINTDELEGATE_H
