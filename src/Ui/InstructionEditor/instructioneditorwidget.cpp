#include <QVBoxLayout>
#include <QHeaderView>

#include "instructioneditorwidget.h"
#include "instructioneditordelegate.h"

#include "appcontext.h"

using namespace InstructionEditor;

 InstructionEditorWidget::InstructionEditorWidget(QWidget* parent)
    : QWidget(parent),
    model(AppContext::instance()->sharedData()->editorMachineState(),
          AppContext::instance()->sharedData()->labels(),
            AppContext::instance()->sharedData()->instructionSet(),
            AppContext::instance()->sharedData()->instructions())
    {
        auto* layout = new QVBoxLayout(this);
        tableView = new QTableView(this);

        delegate = new InstructionEditorDelegate(this);

        tableView->setModel(&model);
        tableView->setItemDelegate(delegate);
        tableView->horizontalHeader()->setStretchLastSection(true);
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        tableView->setEditTriggers(QAbstractItemView::DoubleClicked | QAbstractItemView::EditKeyPressed);

        layout->addWidget(tableView);
    }
