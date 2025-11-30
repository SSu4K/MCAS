#include <QMenuBar>

#include "microcodeeditorwindow.h"

#include "microcodeeditorwidget.h"
#include "jumptableeditorwidget.h"

using namespace MicrocodeEditor;

MicrocodeEditorWindow::MicrocodeEditorWindow(MicrocodeModel* microcodeModel, JumpTableModel* jumpTableModel, QWidget* parent)
    : microcodeModel(microcodeModel),
    jumpTableModel(jumpTableModel),
    microcodeEditor(microcodeModel, parent),
    jumpTableEditor(jumpTableModel),
    EditorWindow(parent)
{
    setWindowTitle(windowTitle());
    resize(1000, 600);

    menuBar()->clear();
    createMenu();
}

void MicrocodeEditorWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

bool MicrocodeEditorWindow::serializeToFile(QFile &file) const {
    QTextStream out(&file);
    microcodeModel->saveToTextStream(out);
    jumpTableModel->saveToTextStream(out);
    return true;
}

bool MicrocodeEditorWindow::serializeFromFile(QFile &file){
    bool success = true;
    QTextStream in1(&file);
    success &= microcodeModel->loadFromTextStream(in1);
    file.seek(0);
    QTextStream in2(&file);
    success &= jumpTableModel->loadFromTextStream(in2);

    return success;
}

void MicrocodeEditorWindow::clearData(){
    microcodeModel->clear();
    jumpTableModel->clear();
}

void MicrocodeEditorWindow::createCustomMenu(){
    setWindowTitle(windowTitle());
    tabWidget.addTab(&microcodeEditor, tr("Microcode"));
    tabWidget.addTab(&jumpTableEditor, tr("Jump Tables"));
    setCentralWidget(&tabWidget);
}
