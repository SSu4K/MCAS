#include "microcodeeditorwindow.h"

#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QCloseEvent>
#include <QFile>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include "microcodeeditorwindow.h"
#include "microcodeeditorwidget.h"
#include "microcodemodel.h"

#include <QMenuBar>
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

MicrocodeEditorWindow::MicrocodeEditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    m_editorWidget = new MicrocodeEditorWidget(this);
    setCentralWidget(m_editorWidget);

    createMenus();
    setWindowTitle("Microcode Editor");
    resize(1000, 600);
}

void MicrocodeEditorWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    // Modern, non-deprecated addAction() syntax
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

    m_editorWidget->m_model->clear();
    m_currentFilePath.clear();
    setWindowTitle("Microcode Editor - [New]");
}

void MicrocodeEditorWindow::openFile()
{
    if (!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, tr("Open Microcode File"),
                                                    "", tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty())
        return;

    if (m_editorWidget->m_model->loadFromTextFile(filePath)) {
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
    m_editorWidget->m_model->saveToTextFile(m_currentFilePath);
}

void MicrocodeEditorWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, tr("Save Microcode File As"),
                                                    m_currentFilePath.isEmpty() ? "microcode.txt" : m_currentFilePath,
                                                    tr("Microcode Files (*.txt *.mc *.tsv)"));
    if (filePath.isEmpty())
        return;

    if (m_editorWidget->m_model->saveToTextFile(filePath)) {
        m_currentFilePath = filePath;
        setWindowTitle(QString("Microcode Editor - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void MicrocodeEditorWindow::exitApp()
{
    if (maybeSave())
        close();
}

// ====================== SAVE / LOAD ======================
bool MicrocodeEditorWindow::maybeSave()
{
    return true; // simplify for now
}

