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
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();

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

void MemoryEditorWindow::createCustomMenu(){

    setWindowTitle(windowTitle());
    QMenu *editMenu = menuBar()->addMenu(tr("&Edit"));

    editMenu->addAction(tr("&Select All"), QKeySequence("Ctrl+A"), &widget, &MemoryEditorWidget::selectAll);
    editMenu->addAction(tr("&Clear"), QKeySequence("Ctrl+Del"), &widget, &MemoryEditorWidget::clearSelected);
    editMenu->addAction(tr("&Random"), QKeySequence("Ctrl+R"), &widget, &MemoryEditorWidget::randomSelected);
    editMenu->addAction(tr("&Fill..."), QKeySequence("Ctrl+F"), &widget, &MemoryEditorWidget::fillSelected);

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

    connect(viewMenu, &QMenu::aboutToShow, this, [=]() {
        switch (model->getUnitSize()) {
            case MemoryUnitSize::Byte:  byteAction->setChecked(true); break;
            case MemoryUnitSize::Half:  halfAction->setChecked(true); break;
            case MemoryUnitSize::Word:  wordAction->setChecked(true); break;
        }
    });

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        if (action == byteAction) widget.setUnitSize(MemoryUnitSize::Byte);
        if (action == halfAction) widget.setUnitSize(MemoryUnitSize::Half);
        if (action == wordAction) widget.setUnitSize(MemoryUnitSize::Word);
    });
}
