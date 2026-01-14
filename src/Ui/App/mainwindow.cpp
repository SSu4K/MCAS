#include <QMenuBar>
#include <QActionGroup>
#include <QLayout>

#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("MCAS Main Window"));
    resize(700, 500);

    createMenu();

    SimulationView *simView = new SimulationView(this);
    setCentralWidget(simView);
}

void MainWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::createToolsMenu(){
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction *openMicrocodeEditorAction = new QAction(tr("Microcode Editor"), this);
    toolsMenu->addAction(openMicrocodeEditorAction);
    connect(openMicrocodeEditorAction, &QAction::triggered, this, &MainWindow::openMicrocodeEditorWindow);

    QAction *openMemoryEditorAction = new QAction(tr("Memory Editor"), this);
    toolsMenu->addAction(openMemoryEditorAction);
    connect(openMemoryEditorAction, &QAction::triggered, this, &MainWindow::openMemoryEditorWindow);

    QAction *openInstructionEditorAction = new QAction(tr("Instruction Editor"), this);
    toolsMenu->addAction(openInstructionEditorAction);
    connect(openInstructionEditorAction, &QAction::triggered, this, &MainWindow::openInstructionEditorWindow);
}

void MainWindow::createViewMenu()
{
    auto viewMenu = menuBar()->addMenu(tr("&View"));
    auto themeMenu = viewMenu->addMenu(tr("Theme"));

    QAction *systemAct = themeMenu->addAction(tr("System Default"));
    QAction *lightAct  = themeMenu->addAction(tr("Light"));
    QAction *darkAct   = themeMenu->addAction(tr("Dark"));

    systemAct->setCheckable(true);
    lightAct->setCheckable(true);
    darkAct->setCheckable(true);

    QActionGroup *group = new QActionGroup(this);
    group->addAction(systemAct);
    group->addAction(lightAct);
    group->addAction(darkAct);
    group->setExclusive(true);

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        if (action == systemAct)    emit setTheme("System");
        if (action == lightAct)     emit setTheme("Light");
        if (action == darkAct)      emit setTheme("Dark");
    });

    QMenu* langMenu = viewMenu->addMenu(tr("Language"));

    QActionGroup* langGroup = new QActionGroup(this);
    QAction* enAction = langMenu->addAction(tr("English"));
    enAction->setCheckable(true);
    QAction* plAction = langMenu->addAction(tr("Polish"));
    plAction->setCheckable(true);

    langGroup->addAction(enAction);
    langGroup->addAction(plAction);
    langGroup->setExclusive(true);

    connect(langGroup, &QActionGroup::triggered, this, [=](QAction *action) {
        if (action == enAction)    emit setLanguage("en");
        if (action == plAction)     emit setLanguage("pl");
    });
}

void MainWindow::createMenu(){
    createToolsMenu();
    createViewMenu();
}

void MainWindow::retranslateUi(){
    setWindowTitle(tr("MCAS Main Window"));
    menuBar()->clear();
    createMenu();
}

