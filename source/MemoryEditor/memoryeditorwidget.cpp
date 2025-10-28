#include "memoryeditorwidget.h"

using namespace MemoryEditor;

MemoryEditorWidget::MemoryEditorWidget(QWidget* parent) : QTableView(parent) {
    m_model = new MemoryModel(this);
    setModel(m_model);
    horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    verticalHeader()->setVisible(false);
    setSelectionMode(QAbstractItemView::NoSelection);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
    setShowGrid(true);
    setAlternatingRowColors(true);
}

void MemoryEditorWidget::setMemory(const QByteArray& memory) {
    m_model->setMemory(memory);
}

void MemoryEditorWidget::resizeEvent(QResizeEvent* event) {
    QTableView::resizeEvent(event);

    int addressColumnWidth = 80; // fixed width
    int cellWidth = 50;           // desired width of each half-word
    int availableWidth = viewport()->width() - addressColumnWidth;
    int cols = qMax(1, availableWidth / cellWidth);

    m_model->setColumns(cols);
    setColumnWidth(0, addressColumnWidth);
    for (int i = 1; i < m_model->columnCount(); ++i)
        setColumnWidth(i, cellWidth);
}
