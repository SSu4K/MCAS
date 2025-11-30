#include <QTableView>
#include <QHeaderView>
#include <QVBoxLayout>
#include <qevent.h>

#include "memoryeditorwidget.h"

#include "Memory/memorymodel.h"
#include "filldialog.h"
#include "Common/hexintdelegate.h"

using namespace MemoryEditor;
MemoryEditorWidget::MemoryEditorWidget(MemoryModel* model, QWidget* parent)
    : model(model), QWidget(parent)
{
    qDebug() << "Begin memory widget init";

    tableView.setModel(model);
    tableView.verticalHeader()->setVisible(true);
    tableView.horizontalHeader()->setSectionResizeMode(QHeaderView::Fixed);
    tableView.setAlternatingRowColors(true);

    tableView.setSelectionBehavior(QAbstractItemView::SelectItems);
    tableView.setSelectionMode(QAbstractItemView::ExtendedSelection);
    tableView.setEditTriggers(
        QAbstractItemView::DoubleClicked |
        QAbstractItemView::EditKeyPressed |
        QAbstractItemView::SelectedClicked |
        QAbstractItemView::AnyKeyPressed
        );

    connect(&tableView, &QTableView::activated, &tableView, [this](const QModelIndex& idx){
        tableView.edit(idx);
    });

    tableView.setItemDelegate(&delegate);

    auto* layout = new QVBoxLayout(this);
    layout->addWidget(&tableView);
    layout->setContentsMargins(0, 0, 0, 0);
    setLayout(layout);

    qDebug() << "End memory widget init";
}

void MemoryEditorWidget::resizeEvent(QResizeEvent* event)
{
    QWidget::resizeEvent(event);
    updateColumnCount();
}

bool MemoryEditorWidget::event(QEvent *e)
{
    if (e->type() == QEvent::Show) {
        // polish widget just before showing it
        qDebug() << "Polishing widget";
        updateColumnCount();
    }
    return QWidget::event(e);
}


void MemoryEditorWidget::updateColumnCount()
{
    if (!tableView.model()) return;
    qDebug() << "Begin updateColumnCount";
    qDebug() << "unitSize:" << (int)model->getUnitSize();
    QFontMetrics fm(tableView.font());
    int charW = fm.horizontalAdvance('F') + 1;
    int hexChars = 2 * (int)model->getUnitSize();
    int padding = 16;
    int cellWidth = hexChars * charW + padding;

    int available = tableView.viewport()->width();
    int cols = qMax(1, available / cellWidth);

    qDebug() << "cols:" << cols;

    if (model->columnCount() != cols) {
        model->setColumns(cols);
        tableView.horizontalHeader()->setDefaultSectionSize(cellWidth);
        tableView.verticalHeader()->setDefaultSectionSize(fm.height() + 8);
    }

    qDebug() << "End updateColumnCount";
}

MemoryUnitSize MemoryEditorWidget::getUnitSize(){
    return model->getUnitSize();
}

void MemoryEditorWidget::setUnitSize(MemoryUnitSize size){
    model->setUnitSize(size);
    delegate.setPrecision(2*(qsizetype)size);
    updateColumnCount();
}

void MemoryEditorWidget::clearData(){
    model->clear();
}

void MemoryEditorWidget::selectAll(){
    tableView.selectAll();
}

void MemoryEditorWidget::clearSelected(){
    auto selectionModel = tableView.selectionModel();
    if (!selectionModel)
        return;

    QModelIndexList indexes = selectionModel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    model->blockSignals(true);

    for (qsizetype i=0; i<indexes.size(); i++) {
        if (indexes[i].flags() & Qt::ItemIsEditable) {
            model->setData(indexes[i], 0, Qt::EditRole);
        }
    }

    model->blockSignals(false);
    tableView.viewport()->update();
}

void MemoryEditorWidget::randomSelected(){
    auto selectionModel = tableView.selectionModel();
    if (!selectionModel)
        return;

    QModelIndexList indexes = selectionModel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    model->blockSignals(true);

    for (qsizetype i=0; i<indexes.size(); i++) {
        // Generate 32-bit, will be truncated to the correct size by the model
        quint32 value = QRandomGenerator::global()->generate();

        if (indexes[i].flags() & Qt::ItemIsEditable) {
            model->setData(indexes[i], value, Qt::EditRole);
        }
    }

    model->blockSignals(false);
    tableView.viewport()->update();
}

void MemoryEditorWidget::fillSelected()
{
    FillDialog dlg(this);
    if (dlg.exec() != QDialog::Accepted)
        return;

    int value = dlg.value();

    auto sel = tableView.selectionModel();
    if (!sel)
        return;

    QModelIndexList indexes = sel->selectedIndexes();
    if (indexes.isEmpty())
        return;

    model->blockSignals(true);

    for (qsizetype i=0; i<indexes.size(); i++) {
        if (indexes[i].flags() & Qt::ItemIsEditable) {
            model->setData(indexes[i], value, Qt::EditRole);
        }
    }

    model->blockSignals(false);
    tableView.viewport()->update();
}
