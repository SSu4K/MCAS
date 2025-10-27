#include "editorwindow.h"

#include <QMenuBar>
#include <QFileDialog>

EditorWindow::EditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    createMenus();
    resize(1000, 600);

    // Move to chidren
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();
}

void EditorWindow::createMenus()
{
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(tr("&New"), QKeySequence::New, this, &EditorWindow::newFile);
    fileMenu->addAction(tr("&Open..."), QKeySequence::Open, this, &EditorWindow::openFile);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("&Save"), QKeySequence::Save, this, &EditorWindow::saveFile);
    fileMenu->addAction(tr("Save &As..."), QKeySequence::SaveAs, this, &EditorWindow::saveFileAs);

    fileMenu->addSeparator();

    fileMenu->addAction(tr("E&xit"), QKeySequence::Quit, this, &EditorWindow::exitApp);
}

void EditorWindow::newFile()
{
    if (!maybeSave())
        return;

    // m_microcodeEditor->m_model->clear();
    // m_jumpTableEditor->model()->clear();
    m_currentFilePath.clear();
    setWindowTitle(windowTitle() + " - [New]");
}

void EditorWindow::openFile()
{
    if (!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, openFilePrompt(),
                                                    "", fileFilterString());
    if (filePath.isEmpty()){
        qDebug("Empty path!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Failed to open!");
        return;
    }

    QTextStream in(&file);
    bool success = serializeFromStream(in);

    qDebug() << "Serializing from" << filePath << "Success:" << success;

    if (success) {
        m_currentFilePath = filePath;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void EditorWindow::saveFile()
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
    QTextStream out(&file);
    success = serializeToStream(out);

    qDebug() << "Serializing to" << m_currentFilePath << "Success:" << success;
}

void EditorWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, saveAsFilePrompt(),
                                                    m_currentFilePath.isEmpty() ? defaultFilename() : m_currentFilePath,
                                                    fileFilterString());
    if (filePath.isEmpty())
        return;

    QFile file(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    bool success = true;
    QTextStream in(&file);
    // success &= m_microcodeEditor->m_model->saveToTextStream(in);
    // success &= m_jumpTableEditor->model()->saveToTextStream(in);

    if (success) {
        m_currentFilePath = filePath;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

void EditorWindow::exitApp()
{
    if (maybeSave())
        close();
}

bool EditorWindow::maybeSave()
{
    return true; // simplify for now
}

