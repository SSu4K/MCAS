#ifndef MICROCODEDELEGATE_H
#define MICROCODEDELEGATE_H

#include <QStyledItemDelegate>

#pragma once
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include "enumdelegate.h"
#include "microcodemodel.h"

class MicrocodeDelegate : public QStyledItemDelegate {
    Q_OBJECT
public:
    explicit MicrocodeDelegate(QObject* parent = nullptr)
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

#endif
