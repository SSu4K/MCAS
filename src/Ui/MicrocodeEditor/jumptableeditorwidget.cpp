#include <QTableView>
#include <QVBoxLayout>
#include <QHeaderView>

#include "jumptableeditorwidget.h"

using namespace Ui;
using namespace Models;

JumpTableEditorWidget::JumpTableEditorWidget(JumpTableModel* jumpTableModel, QWidget* parent)
    : model(jumpTableModel), ZoomWidget(parent)
{
    table.setModel(model);
    table.setAlternatingRowColors(true);
    table.setSelectionBehavior(QAbstractItemView::SelectItems);
    table.setSelectionMode(QAbstractItemView::SingleSelection);
    table.horizontalHeader()->setStretchLastSection(true);

    table.setItemDelegate(&delegate);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(&table);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);

    QTimer::singleShot(0, this, [this]() {
        resizeColumnsToFit();
        table.updateGeometry();
    });
}

void JumpTableEditorWidget::resizeColumnsToFit()
{
    auto *hHeader = table.horizontalHeader();
    const int columnCount = model->columnCount();

    for (int i = 0; i < columnCount; ++i) {
        if (i == columnCount - 1)
            hHeader->setSectionResizeMode(i, QHeaderView::Stretch);
        else
            hHeader->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    table.setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    table.resizeColumnsToContents();
    table.resizeRowsToContents();
}

