#include "breakpointwidget.h"

#include "BreakpointWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

using namespace Ui;

BreakpointWidget::BreakpointWidget(Models::BreakpointModel* model,
                                   QWidget* parent)
    : QWidget(parent),
    model(model)
{
    setupUi();
    setupConnections();
}

void BreakpointWidget::setupUi()
{
    tableView.setModel(model);
    tableView.setItemDelegate(&delegate);
    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);
    tableView.horizontalHeader()->setStretchLastSection(true);

    addButton.setText("Add");
    removeButton.setText("Remove");
    clearButton.setText("Clear");

    auto* buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(&addButton);
    buttonLayout->addWidget(&removeButton);
    buttonLayout->addWidget(&clearButton);
    buttonLayout->addStretch();

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(&tableView);
    layout->addLayout(buttonLayout);

    setLayout(layout);
}

void BreakpointWidget::setupConnections()
{
    connect(&addButton, &QPushButton::clicked, this, [this]() {
        int row = model->rowCount();
        model->insertRow(row);
    });

    connect(&removeButton, &QPushButton::clicked, this, [this]() {
        auto index = tableView.currentIndex();
        if (!index.isValid())
            return;

        model->removeRow(index.row());
    });

    connect(&clearButton, &QPushButton::clicked, this, [this]() {
        model->clear();
    });
}
