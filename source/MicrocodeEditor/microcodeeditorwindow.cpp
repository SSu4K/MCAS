#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QFileDialog>

using namespace MicrocodeEditor;

MicrocodeEditorWindow::MicrocodeEditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_tabWidget = new QTabWidget(this);

    m_microcodeEditor = new MicrocodeEditorWidget(this);
    m_jumpTableEditor = new JumpTableEditorWidget(this);

    m_tabWidget->addTab(m_microcodeEditor, tr("Microcode"));
    m_tabWidget->addTab(m_jumpTableEditor, tr("Jump Tables"));

    setCentralWidget(m_tabWidget);

    createMenus();
    setWindowTitle("Microcode Editor");
    resize(m_microcodeEditor->sizeHint());
}

void MicrocodeEditorWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(tr("&New"), QKeySequence::New, this, &MicrocodeEditorWindow::newFile);
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open, this, &MicrocodeEditorWindow::openFile);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, &MicrocodeEditorWindow::saveFile);
    fileMenu->addAction(tr("Save &As..."), QKeySequence::SaveAs, this, &MicrocodeEditorWindow::saveFileAs);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit, this, &MicrocodeEditorWindow::exitApp);
}

void MicrocodeEditorWindow::newFile()
{
    if (!maybeSave())
        return;

    m_microcodeEditor->m_model->clear();
    m_jumpTableEditor->model()->clear();
    m_currentFilePath.clear();
    setWindowTitle("Microcode Editor - [New]");
}

void MicrocodeEditorWindow::openFile()
{
    if (!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Microcode File"),
                                                    "", tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty()){
        qDebug("Empty path!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Failed to open!");
        return;
    }

    bool success = true;
    QTextStream in1(&file);
    success &= m_microcodeEditor->m_model->loadFromTextStream(in1);
    file.seek(0);
    QTextStream in2(&file);
    success &= m_jumpTableEditor->model()->loadFromTextStream(in2);

    qDebug("Success: %d", success);

    if (success) {
        m_currentFilePath = filePath;
        setWindowTitle(QString("Microcode Editor - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void MicrocodeEditorWindow::saveFile()
{
    if (m_currentFilePath.isEmpty()) {
        saveFileAs();
        return;
    }

    QFile file(m_currentFilePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    bool success = true;
    QTextStream in(&file);
    m_microcodeEditor->m_model->saveToTextStream(in);
    m_jumpTableEditor->model()->saveToTextStream(in);
}

void MicrocodeEditorWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Microcode File As"),
                                                    m_currentFilePath.isEmpty() ? "microcode.txt" : m_currentFilePath,
                                                    tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    bool success = true;
    QTextStream in(&file);
    success &= m_microcodeEditor->m_model->saveToTextStream(in);
    success &= m_jumpTableEditor->model()->saveToTextStream(in);

    if (success) {
        m_currentFilePath = filePath;
        setWindowTitle(QString("Microcode Editor - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void MicrocodeEditorWindow::exitApp()
{
    if (maybeSave())
        close();
}

bool MicrocodeEditorWindow::maybeSave()
{
    return true; // simplify for now
}

