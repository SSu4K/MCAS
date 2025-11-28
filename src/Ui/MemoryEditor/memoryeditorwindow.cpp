#include <QMenuBar>
#include <QActionGroup>

#include "memoryeditorwindow.h"
#include "Memory/memorymodel.h"

using namespace MemoryEditor;

MemoryEditorWindow::MemoryEditorWindow(QWidget *parent)
    : EditorWindow{parent}
{
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();

    m_memoryEditorWidget = new MemoryEditorWidget();
    setCentralWidget(m_memoryEditorWidget);

    menuBar()->clear();
    createMenu();
}

MemoryEditor::MemoryModel* MemoryEditorWindow::getModel(){
    return m_memoryEditorWidget->getModel();
}

void MemoryEditorWindow::openWindow(){
    show();
    // m_memoryEditorWidget->updateColumnCount();
    raise();
    activateWindow();
}


bool MemoryEditorWindow::serializeToFile(QFile &file) const {
    QTextStream out(&file);
    return m_memoryEditorWidget->getModel()->saveToTextStream(out);
}

bool MemoryEditorWindow::serializeFromFile(QFile &file){
    QTextStream in(&file);
    return m_memoryEditorWidget->getModel()->loadFromTextStream(in);
}

void MemoryEditorWindow::clearData(){
    m_memoryEditorWidget->clearData();
}

void MemoryEditorWindow::createCustomMenu(){

    setWindowTitle(windowTitle());
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    editMenu->addAction(tr("&Select All"), QKeySequence("Ctrl+A"), m_memoryEditorWidget, &MemoryEditorWidget::selectAll);
    editMenu->addAction(tr("&Clear"), QKeySequence("Ctrl+Del"), m_memoryEditorWidget, &MemoryEditorWidget::clearSelected);
    editMenu->addAction(tr("&Random"), QKeySequence("Ctrl+R"), m_memoryEditorWidget, &MemoryEditorWidget::randomSelected);
    editMenu->addAction(tr("&Fill..."), QKeySequence("Ctrl+F"), m_memoryEditorWidget, &MemoryEditorWidget::fillSelected);

    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    auto unitSizeMenu = viewMenu->addMenu(tr("&Size"));

    QAction *byteAction = unitSizeMenu->addAction(tr("&Byte"));
    QAction *halfAction  = unitSizeMenu->addAction(tr("&Half"));
    QAction *wordAction   = unitSizeMenu->addAction(tr("&Word"));

    byteAction->setCheckable(true);
    halfAction->setCheckable(true);
    wordAction->setCheckable(true);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(byteAction);
    group->addAction(halfAction);
    group->addAction(wordAction);
    group->setExclusive(true);

    if(m_memoryEditorWidget == nullptr) return;

    connect(viewMenu, &QMenu::aboutToShow, this, [=]() {
        switch (m_memoryEditorWidget->getUnitSize()) {
            case MemoryUnitSize::Byte:  byteAction->setChecked(true); break;
            case MemoryUnitSize::Half:  halfAction->setChecked(true); break;
            case MemoryUnitSize::Word:  wordAction->setChecked(true); break;
        }
    });

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        if (action == byteAction) m_memoryEditorWidget->setUnitSize(MemoryUnitSize::Byte);
        if (action == halfAction) m_memoryEditorWidget->setUnitSize(MemoryUnitSize::Half);
        if (action == wordAction) m_memoryEditorWidget->setUnitSize(MemoryUnitSize::Word);
    });
}
