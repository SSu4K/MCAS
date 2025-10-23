#include "jumptableeditorwidget.h"
#include "MicrocodeEditor/jumptableeditordelegate.h"
#include <qheaderview.h>
#include <QTimer>

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

    m_delegate = new JumpTableEditorDelegate(this);
    m_table->setItemDelegate(m_delegate);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_table);
    layout->setContentsMargins(4, 4, 4, 4);
    setLayout(layout);

    QTimer::singleShot(0, this, [this]() {
        resizeColumnsToFit();
        m_table->updateGeometry();
    });
}

void JumpTableEditorWidget::resizeColumnsToFit()
{
    auto *hHeader = m_table->horizontalHeader();
    const int columnCount = m_model->columnCount();

    for (int i = 0; i < columnCount; ++i) {
        if (i == columnCount - 1)
            hHeader->setSectionResizeMode(i, QHeaderView::Stretch);
        else
            hHeader->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    m_table->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_table->resizeColumnsToContents();
    m_table->resizeRowsToContents();
}

