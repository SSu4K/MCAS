#include "instructioneditorwidget.h"

using namespace InstructionEditor;

 InstructionEditorWidget::InstructionEditorWidget(QWidget* parent)
        : QWidget(parent)
    {
        auto* layout = new QVBoxLayout(this);
        tableView = new QTableView(this);
        model = new InstructionEditorModel(this);
        delegate = new InstructionEditorDelegate(this);

        tableView->setModel(model);
        tableView->setItemDelegate(delegate);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

        auto* addButton = new QPushButton("Add Instruction", this);
        connect(addButton, &QPushButton::clicked, model, [this]() { model->addInstruction(); });

        layout->addWidget(tableView);
        layout->addWidget(addButton);
    }
