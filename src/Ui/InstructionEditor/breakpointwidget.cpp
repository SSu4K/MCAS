#include "breakpointwidget.h"

#include "BreakpointWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QHeaderView>

#include "Common/enumdelegate.h"
#include "Common/hexintdelegate.h"

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

    auto* enumDelegate = new EnumDelegate(this);
    enumDelegate->setItems({"PC", "uAR"});

    auto* hexDelegate = new HexIntDelegate(this);
    hexDelegate->setPrecision(8);

    tableView.setModel(model);
    //tableView.setItemDelegate(&delegate);
    tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);
    tableView.horizontalHeader()->setStretchLastSection(true);

    tableView.setItemDelegateForColumn(1, enumDelegate);
    tableView.setItemDelegateForColumn(2, hexDelegate);

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

void BreakpointWidget::retranslateUi(){
    addButton.setText(tr("Add"));
    removeButton.setText(tr("Remove"));
    clearButton.setText(tr("Clear"));
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
