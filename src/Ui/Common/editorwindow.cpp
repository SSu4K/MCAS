#include <QMenuBar>
#include <QFileDialog>
#include <QCoreApplication>
#include "editorwindow.h"

EditorWindow::EditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1000, 600);

    // connect(AppContext::instance(), &AppContext::languageChanged,
    //         this, &EditorWindow::retranslateUi);

    // Move to chidren
    createMenu();
    setWindowTitle(windowTitle());
}

void EditorWindow::createMenu()
{
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
    menuBar()->clear();
    createMenu();
}

void EditorWindow::newFile()
{
    if (!m_currentFilePath.isEmpty()) saveFile();

    clearData();
    m_currentFilePath.clear();
    setWindowTitle(windowTitle() + " - [New]");
}

bool EditorWindow::openFileFromPath(const QString &path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Failed to open!");
        return false;
    }

    bool success = serializeFromFile(file);

    qDebug() << "Serializing from" << path << "Success:" << success;

    if (success) {
        m_currentFilePath = path;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(path).fileName()));
        return true;
    }
    return false;
}

void EditorWindow::openFile()
{
    saveFile();

    QString path = QFileDialog::getOpenFileName(this, openFilePrompt(),
                                                    "", fileFilterString());
    if (path.isEmpty()){
        qDebug("Empty path!");
        return;
    }

    openFileFromPath(path);

}

void EditorWindow::saveFileToPath(const QString &path){
    QFile file(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)){
        return;
    }

    bool success = true;
    success = serializeToFile(file);

    qDebug() << "Serializing to" << path << "Success:" << success;

    if (success) {
        m_currentFilePath = path;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(path).fileName()));
    }
}

void EditorWindow::saveFile()
{
    if (m_currentFilePath.isEmpty()) {
        saveFileAs();
        return;
    }

    saveFileToPath(m_currentFilePath);
}

void EditorWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, saveAsFilePrompt(),
                                                    m_currentFilePath.isEmpty() ? defaultFilename() : m_currentFilePath,
                                                    fileFilterString());
    if (filePath.isEmpty())
        return;

    saveFileToPath(filePath);
}

void EditorWindow::exitApp()
{
    saveFile();
    close();
}

