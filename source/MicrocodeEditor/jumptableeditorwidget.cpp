#include "jumptableeditorwidget.h"
#include <qheaderview.h>

using namespace MicrocodeEditor;

JumpTableEditorWidget::JumpTableEditorWidget(QWidget* parent)
    : ZoomWidget(parent)
{
    m_table = new QTableView(this);
    m_model = new JumpTableModel(this);

    m_table->setModel(m_model);
    m_table->setAlternatingRowColors(true);
    m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_table->setSelectionMode(QAbstractItemView::SingleSelection);
    m_table->horizontalHeader()->setStretchLastSection(true);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);
}

