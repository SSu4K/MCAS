#include <QMenuBar>
#include <QFileDialog>
#include <QCoreApplication>
#include "Common/appcontext.h"
#include "editorwindow.h"

EditorWindow::EditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1000, 600);

    connect(AppContext::instance(), &AppContext::languageChanged,
            this, &EditorWindow::retranslateUi);

    // Move to chidren
    createMenu();
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();
}

void EditorWindow::createMenu()
{
    const char* ctx = metaObject()->className();
    QMenu* fileMenu = menuBar()->addMenu(tr(FILE_MENU_TEXT));

    fileMenu->addAction(tr(NEW_TEXT), QKeySequence::New, this, &EditorWindow::newFile);
    fileMenu->addAction(tr(OPEN_TEXT), QKeySequence::Open, this, &EditorWindow::openFile);

    fileMenu->addSeparator();
    fileMenu->addAction(tr(SAVE_TEXT), QKeySequence::Save, this, &EditorWindow::saveFile);
    fileMenu->addAction(tr(SAVE_AS_TEXT), QKeySequence::SaveAs, this, &EditorWindow::saveFileAs);

    fileMenu->addSeparator();
    fileMenu->addAction(tr(EXIT_TEXT), QKeySequence::Quit, this, &EditorWindow::exitApp);

    createCustomMenu();
}

void EditorWindow::retranslateUi(){
    qDebug("Retranslating editor");
    menuBar()->clear();
    createMenu();
}

void EditorWindow::newFile()
{
    if (!maybeSave())
        return;

    clearData();
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

    bool success = serializeFromFile(file);

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
    success = serializeToFile(file);

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
    success = serializeToFile(file);

    qDebug() << "Serializing to" << m_currentFilePath << "Success:" << success;

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

