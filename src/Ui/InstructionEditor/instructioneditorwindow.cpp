#include <QMenuBar>
#include "instructioneditorwindow.h"

using namespace Ui;
using namespace Models;

InstructionEditorWindow::InstructionEditorWindow(InstructionModel *instructionModel, Models::BreakpointModel *breakpointModel, QWidget *parent)
    : instructionModel(instructionModel), breakpointModel(breakpointModel),
    instructionEditor(instructionModel), breakpointEditor(breakpointModel), EditorWindow{parent}
{
    updateWindowTitle();
    resize(1000, 600);

    menuBar()->clear();
    createMenu();
}

void InstructionEditorWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

bool InstructionEditorWindow::serializeToFile(QFile& file) const {
    QTextStream out(&file);
    return instructionModel->saveToTextStream(out);
}
bool InstructionEditorWindow::serializeFromFile(QFile& file) {
    QTextStream in(&file);
    return instructionModel->loadFromTextStream(in);
}

void InstructionEditorWindow::clearData(){
    instructionModel->clear();
    return;
}

void InstructionEditorWindow::createCustomMenu() {
    updateWindowTitle();
    instructionTabIndex = tabWidget.addTab(&instructionEditor, "");
    breakpointTabIndex = tabWidget.addTab(&breakpointEditor, "");
    setCentralWidget(&tabWidget);
}

void InstructionEditorWindow::retranslateCustomMenu(){
    tabWidget.setTabText(instructionTabIndex, tr("Instructions"));
    tabWidget.setTabText(breakpointTabIndex, tr("Breakpoints"));
}
