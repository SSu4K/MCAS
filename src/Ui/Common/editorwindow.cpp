#include <QMenuBar>
#include <QFileDialog>
#include <QCoreApplication>
#include "editorwindow.h"

EditorWindow::EditorWindow(QWidget* parent)
    : QMainWindow(parent)
{
    resize(1000, 600);
    m_currentFilePath = "";

    // connect(AppContext::instance(), &AppContext::languageChanged,
    //         this, &EditorWindow::retranslateUi);

    // Move to chidren
    createMenu();
    updateWindowTitle();
}

void EditorWindow::createMenu()
{
    m_fileMenu = menuBar()->addMenu("");

    m_newAction = m_fileMenu->addAction("");
    m_newAction->setShortcut(QKeySequence::New);
    connect(m_newAction, &QAction::triggered, this, &EditorWindow::newFile);

    m_openAction = m_fileMenu->addAction("");
    m_openAction->setShortcut(QKeySequence::Open);
    connect(m_openAction, &QAction::triggered, this, &EditorWindow::openFile);

    m_fileMenu->addSeparator();

    m_saveAction = m_fileMenu->addAction("");
    m_saveAction->setShortcut(QKeySequence::Save);
    connect(m_saveAction, &QAction::triggered, this, &EditorWindow::saveFile);

    m_saveAsAction = m_fileMenu->addAction("");
    m_saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsAction, &QAction::triggered, this, &EditorWindow::saveFileAs);

    m_fileMenu->addSeparator();

    m_exitAction = m_fileMenu->addAction("");
    m_exitAction->setShortcut(QKeySequence::Quit);
    connect(m_exitAction, &QAction::triggered, this, &EditorWindow::exitApp);

    createCustomMenu();
}

void EditorWindow::retranslateUi(){
    updateWindowTitle();

    m_fileMenu->setTitle(tr(FILE_MENU_TEXT));

    m_newAction->setText(tr(NEW_TEXT));
    m_openAction->setText(tr(OPEN_TEXT));
    m_saveAction->setText(tr(SAVE_TEXT));
    m_saveAsAction->setText(tr(SAVE_AS_TEXT));
    m_exitAction->setText(tr(EXIT_TEXT));
    retranslateCustomMenu();
}

void EditorWindow::updateWindowTitle(){
    if(m_currentFilePath.isEmpty()){
        setWindowTitle(windowTitle() + " - [" + tr("New") + "]");
    }
    else if(!QFileInfo::exists(m_currentFilePath)){
        setWindowTitle(windowTitle() + " - [" + tr("New") + "]");
    }
    else{
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(m_currentFilePath).fileName()));
    }
}

void EditorWindow::newFile()
{
    clearData();
    m_currentFilePath.clear();
    updateWindowTitle();
}

bool EditorWindow::openFileFromPath(const QString &path){
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Failed to open!");
        return false;
    }

    bool success = serializeFromFile(file);

    qDebug() << "Serializing from:" << path << "Success:" << success;

    if (success) {
        if(path != m_currentFilePath){
            emit fileChanged(path);
            m_currentFilePath = path;
            updateWindowTitle();
        }
        return true;
    }
    return false;
}

void EditorWindow::openFile()
{
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

    qDebug() << "Serializing to:" << path << "Success:" << success;

    if (success) {
        if(path != m_currentFilePath){
            emit fileChanged(path);
            m_currentFilePath = path;
            setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(path).fileName()));
        }
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
    close();
}

