#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QFileDialog>

using namespace MicrocodeEditor;

MicrocodeEditorWindow::MicrocodeEditorWindow(QWidget* parent)
    : EditorWindow(parent)
{
    m_microcodeEditor = new MicrocodeEditorWidget(this);
    m_jumpTableEditor = new JumpTableEditorWidget(this);

    setWindowTitle(windowTitle());
    resize(1000, 600);

    menuBar()->clear();
    createMenu();
}

bool MicrocodeEditorWindow::serializeToFile(QFile &file) const {
    QTextStream out(&file);
    m_microcodeEditor->model()->saveToTextStream(out);
    m_jumpTableEditor->model()->saveToTextStream(out);
    return true;
}

bool MicrocodeEditorWindow::serializeFromFile(QFile &file){
    bool success = true;
    QTextStream in1(&file);
    success &= m_microcodeEditor->model()->loadFromTextStream(in1);
    file.seek(0);
    QTextStream in2(&file);
    success &= m_jumpTableEditor->model()->loadFromTextStream(in2);

    return success;
}

void MicrocodeEditorWindow::clearData(){
    m_microcodeEditor->model()->clear();
    m_jumpTableEditor->model()->clear();
}

void MicrocodeEditorWindow::createCustomMenu(){
    setWindowTitle(windowTitle());
    m_tabWidget = new QTabWidget(this);
    m_tabWidget->addTab(m_microcodeEditor, tr("Microcode"));
    m_tabWidget->addTab(m_jumpTableEditor, tr("Jump Tables"));
    setCentralWidget(m_tabWidget);
}
