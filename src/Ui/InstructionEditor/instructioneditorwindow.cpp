#include <QMenuBar>
#include "instructioneditorwindow.h"
#include "instructioneditorwidget.h"

using namespace InstructionEditor;

InstructionEditorWindow::InstructionEditorWindow(QWidget *parent)
    : EditorWindow{parent}
{
    // setWindowTitle(windowTitle());
    menuBar()->clear();
    createMenu();

    m_widget = new InstructionEditorWidget();
    if(m_widget){
        setCentralWidget(m_widget);
    }
    else{
        qDebug() << "Failed to init InstructionEditorWidget";
    }
}

InstructionEditor::InstructionEditorModel* InstructionEditorWindow::getModel() {return m_widget->getModel();}

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
