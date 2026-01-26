#include <QHeaderView>
#include <QVBoxLayout>

#include "instructionseteditorwidget.h"
#include "Instruction/instructionsetmodel.h"

namespace Ui {

InstructionSetEditorWidget::InstructionSetEditorWidget(Models::InstructionSetModel* instructionSetModel, QWidget* parent)
    : model(instructionSetModel),
      ZoomWidget(parent)
{

    tableView.setAlternatingRowColors(true);

    // Selection & editing behavior
    tableView.setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView.setSelectionMode(QAbstractItemView::SingleSelection);
    tableView.setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked |
        QAbstractItemView::AnyKeyPressed
        );

    connect(&tableView, &QTableView::activated, this, [this](const QModelIndex& idx){
        tableView.edit(idx);
    });

    // Model
    tableView.setModel(model);

    // Delegate for non-enum columns
    // tableView.setItemDelegate(&delegate);

    // Header setup
    auto* hHeader = tableView.horizontalHeader();
    hHeader->setStretchLastSection(true);
    hHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    hHeader->setMinimumSectionSize(80);
    tableView.verticalHeader()->setVisible(false);

    // Layout
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(&tableView);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(2);
    setLayout(layout);
}

}
