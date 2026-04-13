#include <QMenuBar>
#include <QActionGroup>

#include "memoryeditorwindow.h"
#include "Memory/memorymodel.h"

using namespace Ui;
using namespace Machine;
using namespace Models;

MemoryEditorWindow::MemoryEditorWindow(MemoryModel* model, QWidget *parent)
    : model(model), widget(model), EditorWindow{parent}
{
    updateWindowTitle();

    setCentralWidget(&widget);

    menuBar()->clear();
    createMenu();
}

void MemoryEditorWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}


bool MemoryEditorWindow::serializeToFile(QFile &file) const {
    QTextStream out(&file);
    return model->saveToTextStream(out);
}

bool MemoryEditorWindow::serializeFromFile(QFile &file){
    QTextStream in(&file);
    return model->loadFromTextStream(in);
}

void MemoryEditorWindow::clearData(){
    widget.clearData();
}

void MemoryEditorWindow::createCustomMenu()
{
    // --- Edit menu ---
    m_editMenu = menuBar()->addMenu("");

    m_selectAllAction = m_editMenu->addAction("");
    m_selectAllAction->setShortcut(QKeySequence("Ctrl+A"));
    connect(m_selectAllAction, &QAction::triggered, &widget, &MemoryEditorWidget::selectAll);

    m_clearAction = m_editMenu->addAction("");
    m_clearAction->setShortcut(QKeySequence("Ctrl+Del"));
    connect(m_clearAction, &QAction::triggered, &widget, &MemoryEditorWidget::clearSelected);

    m_randomAction = m_editMenu->addAction("");
    m_randomAction->setShortcut(QKeySequence("Ctrl+R"));
    connect(m_randomAction, &QAction::triggered, &widget, &MemoryEditorWidget::randomSelected);

    m_fillAction = m_editMenu->addAction("");
    m_fillAction->setShortcut(QKeySequence("Ctrl+F"));
    connect(m_fillAction, &QAction::triggered, &widget, &MemoryEditorWidget::fillSelected);

    // --- View menu ---
    m_viewMenu = menuBar()->addMenu("");

    m_unitSizeMenu = m_viewMenu->addMenu("");

    m_byteAction = m_unitSizeMenu->addAction("");
    m_halfAction = m_unitSizeMenu->addAction("");
    m_wordAction = m_unitSizeMenu->addAction("");

    m_byteAction->setCheckable(true);
    m_halfAction->setCheckable(true);
    m_wordAction->setCheckable(true);

    m_unitSizeGroup = new QActionGroup(this);
    m_unitSizeGroup->addAction(m_byteAction);
    m_unitSizeGroup->addAction(m_halfAction);
    m_unitSizeGroup->addAction(m_wordAction);
    m_unitSizeGroup->setExclusive(true);

    // --- State sync ---
    connect(m_viewMenu, &QMenu::aboutToShow, this, [this]() {
        switch (model->getUnitSize()) {
        case MemoryUnitSize::Byte:  m_byteAction->setChecked(true); break;
        case MemoryUnitSize::Half:  m_halfAction->setChecked(true); break;
        case MemoryUnitSize::Word:  m_wordAction->setChecked(true); break;
        }
    });

    connect(m_unitSizeGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        if (action == m_byteAction) widget.setUnitSize(MemoryUnitSize::Byte);
        if (action == m_halfAction) widget.setUnitSize(MemoryUnitSize::Half);
        if (action == m_wordAction) widget.setUnitSize(MemoryUnitSize::Word);
    });
}

void MemoryEditorWindow::retranslateCustomMenu()
{
    m_editMenu->setTitle(tr("&Edit"));

    m_selectAllAction->setText(tr("&Select All"));
    m_clearAction->setText(tr("&Clear"));
    m_randomAction->setText(tr("&Random"));
    m_fillAction->setText(tr("&Fill..."));

    m_viewMenu->setTitle(tr("&View"));
    m_unitSizeMenu->setTitle(tr("&Size"));

    m_byteAction->setText(tr("&Byte"));
    m_halfAction->setText(tr("&Half"));
    m_wordAction->setText(tr("&Word"));
}
