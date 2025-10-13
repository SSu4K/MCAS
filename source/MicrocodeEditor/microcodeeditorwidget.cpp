#include "microcodeeditorwidget.h"
#include "microcode.h"
#include "microcodeeditordelegate.h"
#include <QHeaderView>
#include <QVBoxLayout>
#include <QTimer>

using namespace MicrocodeEditor;

MicrocodeEditorWidget::MicrocodeEditorWidget(QWidget* parent)
    : ZoomWidget(parent)
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
    code.instructions.append(instr);

    m_model->setMicrocode(&code);

    // Adjust column widths
    resizeColumnsToFit();

    connect(m_model, &QAbstractItemModel::dataChanged,
            this, &MicrocodeEditorWidget::resizeColumnsToFit);
    connect(m_model, &QAbstractItemModel::layoutChanged,
            this, &MicrocodeEditorWidget::resizeColumnsToFit);
    connect(m_model, &QAbstractItemModel::modelReset,
            this, &MicrocodeEditorWidget::resizeColumnsToFit);

    QTimer::singleShot(0, this, [this]() {
        m_tableView->resizeColumnsToContents();
    });
}


void MicrocodeEditorWidget::resizeColumnsToFit()
{
    auto *hHeader = m_tableView->horizontalHeader();
    const int columnCount = m_model->columnCount();

    for (int i = 0; i < columnCount; ++i) {
        if (i == columnCount - 1)
            hHeader->setSectionResizeMode(i, QHeaderView::Stretch);
        else
            hHeader->setSectionResizeMode(i, QHeaderView::ResizeToContents);
    }

    m_tableView->setSizeAdjustPolicy(QAbstractScrollArea::AdjustIgnored);
    m_tableView->resizeColumnsToContents();
    m_tableView->resizeRowsToContents();
}


void MicrocodeEditorWidget::onZoomChanged(double factor)
{
    QFont font = m_tableView->font();
    font.setPointSizeF(baseFontSize * factor);

    m_tableView->setFont(font);
    m_tableView->horizontalHeader()->setFont(font);
    m_tableView->verticalHeader()->setFont(font);

    QTimer::singleShot(0, this, [this, factor]() {
        m_tableView->horizontalHeader()->setMinimumSectionSize(int(10 * factor));
        resizeColumnsToFit();
        m_tableView->updateGeometry();
    });
}

