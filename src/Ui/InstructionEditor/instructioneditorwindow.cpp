#include <QMenuBar>
#include "instructioneditorwindow.h"

using namespace Ui;
using namespace Models;

InstructionEditorWindow::InstructionEditorWindow(InstructionModel *model, QWidget *parent)
    : model(model), widget(model), EditorWindow{parent}
{
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
    QTextStream out(&file);
    return model->saveToTextStream(out);
}
bool InstructionEditorWindow::serializeFromFile(QFile& file) {
    QTextStream in(&file);
    return model->loadFromTextStream(in);
}

void InstructionEditorWindow::clearData(){
    model->clear();
    return;
}

void InstructionEditorWindow::createCustomMenu() {
    updateWindowTitle();
}

void InstructionEditorWindow::retranslateCustomMenu(){

}
