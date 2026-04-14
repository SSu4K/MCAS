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
    instructionModel->saveToTextStream(out);
    breakpointModel->saveToTextStream(out);
    return true;
}
bool InstructionEditorWindow::serializeFromFile(QFile& file) {
    bool success = true;
    QTextStream in1(&file);
    success &= instructionModel->loadFromTextStream(in1);
    file.seek(0);
    QTextStream in2(&file);
    success &= breakpointModel->loadFromTextStream(in2);

    return success;
}

void InstructionEditorWindow::clearData(){
    instructionModel->clear();
    breakpointModel->clear();
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

    breakpointEditor.retranslateUi();
}
