#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>

#include "memoryeditorwidget.h"

#include "memorymodel.h"
#include "filldialog.h"
#include "Common/hexintdelegate.h"
#include "Common/appcontext.h"

using namespace MemoryEditor;
MemoryEditorWidget::MemoryEditorWidget(QWidget* parent)
    : QWidget(parent)
{
    m_tableView = new QTableView(this);
    m_model = AppContext::instance()->sharedData()->memory().get();

    m_tableView->setModel(m_model);
    m_tableView->verticalHeader()->setVisible(true);
    m_tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    m_tableView->setAlternatingRowColors(true);

    m_tableView->setSelectionBehavior(QAbstractItemView::SelectItems);
    m_tableView->setSelectionMode(QAbstractItemView::ExtendedSelection);
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
    int charW = fm.horizontalAdvance('F') + 1;
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

void MemoryEditorWidget::selectAll(){
    m_tableView->selectAll();
}

void MemoryEditorWidget::clearSelected(){
    auto selectionModel = m_tableView->selectionModel();
    if (!selectionModel)
        return;

    QModelIndexList indexes = selectionModel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    m_model->blockSignals(true);

    for (qsizetype i=0; i<indexes.size(); i++) {
        if (indexes[i].flags() & Qt::ItemIsEditable) {
            m_model->setData(indexes[i], 0, Qt::EditRole);
            emit m_model->dataChanged(indexes[i], indexes[i], {Qt::EditRole});
        }
    }

    m_model->blockSignals(false);
    m_tableView->viewport()->update();
}

void MemoryEditorWidget::randomSelected(){
    auto selectionModel = m_tableView->selectionModel();
    if (!selectionModel)
        return;

    QModelIndexList indexes = selectionModel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    m_model->blockSignals(true);

    for (qsizetype i=0; i<indexes.size(); i++) {
        quint32 max = unitMasks[(int)m_model->getUnitSize()];
        quint32 value = QRandomGenerator::global()->bounded(max);

        if (indexes[i].flags() & Qt::ItemIsEditable) {
            m_model->setData(indexes[i], value, Qt::EditRole);
            emit m_model->dataChanged(indexes[i], indexes[i], {Qt::EditRole});
        }
    }

    m_model->blockSignals(false);
    m_tableView->viewport()->update();
}

void MemoryEditorWidget::fillSelected()
{
    FillDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    int value = dlg.value();

    auto sel = m_tableView->selectionModel();
    if (!sel)
        return;

    QModelIndexList indexes = sel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    m_model->blockSignals(true);

    for (const QModelIndex& idx : indexes) {
        if (!(idx.flags() & Qt::ItemIsEditable))
            continue;
        m_model->setData(idx, value, Qt::EditRole);
    }

    m_model->blockSignals(false);
    m_tableView->viewport()->update();
}
