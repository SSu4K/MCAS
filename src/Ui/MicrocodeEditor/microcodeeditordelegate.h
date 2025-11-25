#ifndef MICROCODEEDITORDELEGATE_H
#define MICROCODEEDITORDELEGATE_H

#include <QStyledItemDelegate>

namespace MicrocodeEditor{

    class MicrocodeEditorDelegate : public QStyledItemDelegate {
        Q_OBJECT
    public:
        explicit MicrocodeEditorDelegate(QObject* parent = nullptr)
            : QStyledItemDelegate(parent) {}

        QWidget* createEditor(QWidget* parent,
                              const QStyleOptionViewItem& option,
                              const QModelIndex& index) const override;

        bool eventFilter(QObject* editor, QEvent* event) override;

        void setEditorData(QWidget* editor, const QModelIndex& index) const override;

        void setModelData(QWidget* editor, QAbstractItemModel* model,
                          const QModelIndex& index) const override;

        void updateEditorGeometry(QWidget* editor,
                                  const QStyleOptionViewItem& option,
                                  const QModelIndex&) const override;

    private:
        mutable QWidget* m_currentEditor = nullptr;

    };

}

#endif
