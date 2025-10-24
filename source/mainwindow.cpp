#include "mainwindow.h"
#include "MicrocodeEditor/microcodeeditorwindow.h"
#include "appcontext.h"
#include "mcasapp.h"
#include <QMenuBar>
#include <QAction>
#include <QActionGroup>
#include <QEvent>

using namespace MicrocodeEditor;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle("Main Application");
    resize(700, 500);

    createToolsMenu();
    createViewMenu();
}

void MainWindow::openMicrocodeEditorWindow()
{
    if (!m_microcodeEditorWindow) {
        m_microcodeEditorWindow = new MicrocodeEditorWindow(this);
        m_microcodeEditorWindow->installEventFilter(this);
    }

    m_microcodeEditorWindow->show();
    m_microcodeEditorWindow->raise();
    m_microcodeEditorWindow->activateWindow();
}

void MainWindow::closeMicrocodeEditorWindow(){
    m_microcodeEditorWindow = nullptr;
}


void MainWindow::createToolsMenu(){
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));
    QAction *openEditorAction = new QAction(tr("Open Microcode Editor"), this);
    toolsMenu->addAction(openEditorAction);
    connect(openEditorAction, &QAction::triggered, this, &MainWindow::openMicrocodeEditorWindow);
}

void MainWindow::createViewMenu()
{
    auto viewMenu = menuBar()->addMenu(tr("&View"));
    auto themeMenu = viewMenu->addMenu(tr("&Theme"));

    QAction *systemAct = themeMenu->addAction(tr("System Default"));
    QAction *lightAct  = themeMenu->addAction(tr("Light"));
    QAction *darkAct   = themeMenu->addAction(tr("Dark"));

    QActionGroup *group = new QActionGroup(this);
    group->addAction(systemAct);
    group->addAction(lightAct);
    group->addAction(darkAct);
    group->setExclusive(true);

    // Reflect current theme
    auto *context = AppContext::instance();
    switch (context->currentTheme()) {
    case AppContext::Theme::System: systemAct->setChecked(true); break;
    case AppContext::Theme::Light: lightAct->setChecked(true); break;
    case AppContext::Theme::Dark: darkAct->setChecked(true); break;
    }

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        auto *app = static_cast<MCASApp*>(QApplication::instance());
        AppContext::Theme newTheme = AppContext::Theme::System;
        if (action == lightAct) newTheme = AppContext::Theme::Light;
        if (action == darkAct)  newTheme = AppContext::Theme::Dark;

        context->setTheme(newTheme);
        app->initPalette();
    });
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

