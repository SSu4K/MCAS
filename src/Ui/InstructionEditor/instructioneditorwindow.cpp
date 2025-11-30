#include <QMenuBar>
#include "instructioneditorwindow.h"

using namespace Ui;
using namespace Models;

InstructionEditorWindow::InstructionEditorWindow(InstructionModel *model, QWidget *parent)
    : model(model), widget(model), EditorWindow{parent}
{
    // setWindowTitle(windowTitle());
    menuBar()->clear();
    createMenu();

    setCentralWidget(&widget);
}

void InstructionEditorWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

bool InstructionEditorWindow::serializeToFile(QFile& file) const {
    // do nothing for now
    return true;
}
bool InstructionEditorWindow::serializeFromFile(QFile& file) {
    // do nothing for now
    return true;
}

void InstructionEditorWindow::clearData(){
    // do nothing for now
    return;
}

void InstructionEditorWindow::createCustomMenu() {
    setWindowTitle(windowTitle());
    return;
}
