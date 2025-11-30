#include <QVBoxLayout>
#include <QHeaderView>

#include "instructioneditorwidget.h"

using namespace Ui;
using namespace Models;

 InstructionEditorWidget::InstructionEditorWidget(InstructionModel *model, QWidget* parent)
    : QWidget(parent),model(model)
    {
        auto* layout = new QVBoxLayout(this);

        tableView.setModel(model);
        tableView.setItemDelegate(&delegate);
        tableView.horizontalHeader()->setStretchLastSection(true);
        tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView.setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

        layout->addWidget(&tableView);
    }
