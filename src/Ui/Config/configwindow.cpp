#include <QMenuBar>

#include "configwindow.h"
#include "Instruction/instructionsetmodel.h"

using namespace Ui;
using Models::InstructionSetModel;

ConfigWindow::ConfigWindow(InstructionSetModel* instructionSetModel, QWidget* parent)
    : instructionSetModel(instructionSetModel),
    instructionSetEditorWidget(instructionSetModel, this),
    EditorWindow(parent)
{
    setWindowTitle(windowTitle());
    resize(1000, 600);

    menuBar()->clear();
    createMenu();
}

void ConfigWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

bool ConfigWindow::serializeToFile(QFile &file) const {
    QTextStream out(&file);
    instructionSetModel->saveToTextStream(out);
    return true;
}

bool ConfigWindow::serializeFromFile(QFile &file){
    bool success = true;
    QTextStream in1(&file);
    success &= instructionSetModel->loadFromTextStream(in1);
    m_currentFilePath = file.fileName();

    return success;
}

void ConfigWindow::clearData(){
    instructionSetModel->clear();
}

void ConfigWindow::createCustomMenu(){
    setWindowTitle(windowTitle());
    setCentralWidget(&instructionSetEditorWidget);
    // widget here
}
