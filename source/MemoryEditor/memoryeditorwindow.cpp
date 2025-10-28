#include "memoryeditorwindow.h"
#include <qmenubar.h>

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

bool MemoryEditorWindow::serializeToFile(QFile &file) const {
    // do nothing for now
    return true;
}

bool MemoryEditorWindow::serializeFromFile(QFile &file){
    // do nothing for now
    return true;
}

void MemoryEditorWindow::clearData(){
    // do nothing for now
}

void MemoryEditorWindow::createCustomMenu(){
    QMenu *viewMenu = menuBar()->addMenu(tr("&View"));

    auto unitSize = viewMenu->addMenu(tr("Size"));

    QAction *byteAction = unitSize->addAction(tr("Byte"));
    QAction *halfAction  = unitSize->addAction(tr("Half"));
    QAction *wordAction   = unitSize->addAction(tr("Word"));

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
            case MemoryEditor::MemoryUnitSize::Byte:  byteAction->setChecked(true); break;
            case MemoryEditor::MemoryUnitSize::Half:  halfAction->setChecked(true); break;
            case MemoryEditor::MemoryUnitSize::Word:  wordAction->setChecked(true); break;
        }
    });

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        if (action == byteAction) m_memoryEditorWidget->setUnitSize(MemoryEditor::MemoryUnitSize::Byte);
        if (action == halfAction) m_memoryEditorWidget->setUnitSize(MemoryEditor::MemoryUnitSize::Half);
        if (action == wordAction) m_memoryEditorWidget->setUnitSize(MemoryEditor::MemoryUnitSize::Word);
    });


}
