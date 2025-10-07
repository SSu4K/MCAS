#ifndef ENUMDELEGATE_H
#define ENUMDELEGATE_H

#include <QStyledItemDelegate>
#include <QComboBox>
#include <QCompleter>
#include <QStringListModel>

class EnumDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit EnumDelegate(QObject* parent = nullptr)
        : QStyledItemDelegate(parent) {}

    void setItems(const QStringList& items);

    QWidget* createEditor(QWidget* parent,
                          const QStyleOptionViewItem&,
                          const QModelIndex&) const override;

    //bool eventFilter(QObject* watched, QEvent* event) override;

    void setEditorData(QWidget* editor, const QModelIndex& index) const override;

    void setModelData(QWidget* editor,
                      QAbstractItemModel* model,
                      const QModelIndex& index) const override;

    void updateEditorGeometry(QWidget* editor,
                              const QStyleOptionViewItem& option,
                              const QModelIndex&) const override;

    void paint(QPainter* painter,
               const QStyleOptionViewItem& option,
               const QModelIndex& index) const override;

private:
    QStringList m_items;
};

#endif // ENUMDELEGATE_H
