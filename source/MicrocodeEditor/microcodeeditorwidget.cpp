#include "microcodeeditorwidget.h"
#include "microcode.h"
#include "microcodeeditordelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>

using namespace MicrocodeEditor;

MicrocodeEditorWidget::MicrocodeEditorWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tableView = new QTableView(this);
    m_tableView->setAlternatingRowColors(true);

    // Selection & editing behavior
    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked |
        QAbstractItemView::AnyKeyPressed
        );

    connect(m_tableView, &QTableView::activated, this, [this](const QModelIndex& idx){
        m_tableView->edit(idx);
    });

    // Model
    m_model = new MicrocodeModel(this);
    m_tableView->setModel(m_model);

    // Delegate for non-enum columns
    m_delegate = new MicrocodeEditorDelegate(this);
    m_tableView->setItemDelegate(m_delegate);

    // Header setup
    auto* hHeader = m_tableView->horizontalHeader();
    hHeader->setStretchLastSection(true);
    hHeader->setSectionResizeMode(QHeaderView::ResizeToContents);
    hHeader->setMinimumSectionSize(80);
    m_tableView->verticalHeader()->setVisible(false);

    // Layout
    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_tableView);
    layout->setContentsMargins(4, 4, 4, 4);
    layout->setSpacing(2);
    setLayout(layout);

    // Initial microcode
    Microcode code;
    Instruction instr;
    // instr.address = "0x0";
    // instr.label   = "START";
    // instr.alu     = "ADD";
    // instr.s1      = "A";
    // instr.s2      = "B";
    // instr.dest    = "C";
    // instr.constant= "0x0";
    // instr.jcond   = "TRUE";
    code.instructions.append(instr);

    m_model->setMicrocode(&code);

    // Adjust column widths
    resizeColumnsToFit();
}


void MicrocodeEditorWidget::resizeColumnsToFit() {
    auto* hHeader = m_tableView->horizontalHeader();
    hHeader->setSectionResizeMode(QHeaderView::Stretch);
    m_tableView->resizeColumnsToContents();
}
