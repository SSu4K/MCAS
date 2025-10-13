#include "mainwindow.h"
#include "MicrocodeEditor/microcodeeditorwindow.h"
#include <QMenuBar>
#include <QAction>
#include <QEvent>

using namespace MicrocodeEditor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Main Application");
    resize(700, 500);

    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction *openEditorAction = new QAction(tr("Open Microcode Editor"), this);
    toolsMenu->addAction(openEditorAction);

    connect(openEditorAction, &QAction::triggered, this, &MainWindow::openMicrocodeEditorWindow);
}

void MainWindow::openMicrocodeEditorWindow()
{
    if (!m_microcodeEditorWindow) {
        m_microcodeEditorWindow = new MicrocodeEditorWindow(this);

        // m_microcodeEditorWindow->setAttribute(Qt::WA_DeleteOnClose);
        // connect(m_microcodeEditorWindow, &QObject::destroyed, this, &MainWindow::closeMicrocodeEditorWindow);
        m_microcodeEditorWindow->installEventFilter(this);
    }

    m_microcodeEditorWindow->show();
    m_microcodeEditorWindow->raise();
    m_microcodeEditorWindow->activateWindow();
}

void MainWindow::closeMicrocodeEditorWindow(){
    m_microcodeEditorWindow = nullptr;
}

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_microcodeEditorWindow && event->type() == QEvent::Close) {
        m_microcodeEditorWindow->hide();
        event->ignore();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

