#include "memoryeditorwidget.h"

using namespace MemoryEditor;
MemoryEditorWidget::MemoryEditorWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tableView = new QTableView(this);
    m_model = new MemoryModel(this);

    m_tableView->setModel(m_model);
    m_tableView->verticalHeader()->setVisible(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->setAlternatingRowColors(true);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    m_tableView->setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked |
        QAbstractItemView::AnyKeyPressed
        );

    connect(m_tableView, &QTableView::activated, m_tableView, [this](const QModelIndex& idx){
        m_tableView->edit(idx);
    });

    m_delegate = new HexIntDelegate(this);
    m_tableView->setItemDelegate(m_delegate);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(m_tableView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    QTimer::singleShot(0, this, [this]{ updateColumnCount(); });
}

void MemoryEditorWidget::setMemory(const QByteArray& memory) {
    m_model->setMemory(memory);
}

void MemoryEditorWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateColumnCount();
}


void MemoryEditorWidget::updateColumnCount()
{
    if (!m_tableView->model()) return;

    QFontMetrics fm(m_tableView->font());
    int charW = fm.horizontalAdvance('F');
    int hexChars = 2 * (int)m_model->getUnitSize();
    int padding = 16;
    int cellWidth = hexChars * charW + padding;

    int available = m_tableView->viewport()->width();
    int cols = qMax(1, available / cellWidth);

    if (m_model->columnCount() != cols) {
        m_model->setColumns(cols);
        m_tableView->horizontalHeader()->setDefaultSectionSize(cellWidth);
        m_tableView->verticalHeader()->setDefaultSectionSize(fm.height() + 8);
    }
}

MemoryUnitSize MemoryEditorWidget::getUnitSize(){
    return m_model->getUnitSize();
}

void MemoryEditorWidget::setUnitSize(MemoryUnitSize size){
    m_model->setUnitSize(size);
    m_delegate->setPrecision(2*(qsizetype)size);
    updateColumnCount();
}

void MemoryEditorWidget::clearData(){
    m_model->clear();
}
