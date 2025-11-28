#include <QMenuBar>
#include <QActionGroup>

#include "mainwindow.h"
#include "appcontext.h"
#include "mcasapp.h"

#include "Instruction/instructioneditormodel.h"

using namespace MicrocodeEditor;
using namespace MemoryEditor;
using namespace InstructionEditor;


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("MCAS Main Window"));

    setWindowIcon(QIcon(":/icons/appicon.png"));

    resize(700, 500);

    createMenu();

    connect(AppContext::instance(), &AppContext::languageChanged,
            this, &MainWindow::retranslateUi);

    m_memoryEditorWindow = new MemoryEditorWindow(this);
    m_instructionEditorWindow = new InstructionEditorWindow(this);

    auto memoryModel = m_memoryEditorWindow->getModel();
    auto instructionModel = m_instructionEditorWindow->getModel();
    connect(memoryModel, &MemoryModel::memoryRegionChanged, instructionModel, &InstructionEditorModel::onMemoryRegionChanged);
    connect(instructionModel, &InstructionEditorModel::memoryRegionChanged, memoryModel, &MemoryModel::onMemoryRegionChanged);
}

MainWindow::~MainWindow(){
    closeInstructionEditorWindow();
    closeMemoryEditorWindow();
    closeMicrocodeEditorWindow();
    AppContext::freeInstance();
    QMainWindow::~QMainWindow();
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
    m_microcodeEditorWindow->close();
    m_microcodeEditorWindow = nullptr;
}

void MainWindow::openMemoryEditorWindow(){
    if (!m_memoryEditorWindow) {
        m_memoryEditorWindow = new MemoryEditorWindow(this);
        m_memoryEditorWindow->installEventFilter(this);
    }

    m_memoryEditorWindow->show();
    m_memoryEditorWindow->raise();
    m_memoryEditorWindow->activateWindow();
}

void MainWindow::closeMemoryEditorWindow(){
    m_memoryEditorWindow->close();
    m_memoryEditorWindow = nullptr;
}

void MainWindow::openInstructionEditorWindow(){
    if (!m_instructionEditorWindow) {
        m_instructionEditorWindow = new InstructionEditorWindow(this);
        m_instructionEditorWindow->installEventFilter(this);
    }

    m_instructionEditorWindow->show();
    m_instructionEditorWindow->raise();
    m_instructionEditorWindow->activateWindow();
}

void MainWindow::closeInstructionEditorWindow(){
    m_instructionEditorWindow->close();
    m_instructionEditorWindow = nullptr;
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

    // Reflect current theme
    auto *context = AppContext::instance();

    connect(viewMenu, &QMenu::aboutToShow, this, [=]() {
        switch (context->currentTheme()) {
        case AppContext::Theme::System: systemAct->setChecked(true); break;
        case AppContext::Theme::Light:  lightAct->setChecked(true);  break;
        case AppContext::Theme::Dark:   darkAct->setChecked(true);   break;
        }
    });

    connect(group, &QActionGroup::triggered, this, [=](QAction *action) {
        auto *app = static_cast<MCASApp*>(QApplication::instance());
        AppContext::Theme newTheme = AppContext::Theme::System;
        if (action == lightAct) newTheme = AppContext::Theme::Light;
        if (action == darkAct)  newTheme = AppContext::Theme::Dark;

        context->setTheme(newTheme);
        app->initPalette();
    });

    QMenu* langMenu = viewMenu->addMenu(tr("Language"));

    QActionGroup* langGroup = new QActionGroup(this);
    QAction* enAction = langMenu->addAction(tr("English"));
    enAction->setCheckable(true);
    QAction* plAction = langMenu->addAction(tr("Polish"));
    plAction->setCheckable(true);

    langGroup->addAction(enAction);
    langGroup->addAction(plAction);

    connect(plAction, &QAction::triggered, this, []{
        AppContext::instance()->setLanguage(AppContext::Language::Polish);
    });
    connect(enAction, &QAction::triggered, this, []{
        AppContext::instance()->setLanguage(AppContext::Language::English);
    });

    connect(viewMenu, &QMenu::aboutToShow, this, [=]() {
        switch (context->currentLanguage()) {
            case AppContext::Language::English: enAction->setChecked(true); break;
            case AppContext::Language::Polish:  plAction->setChecked(true);  break;
            default: break;
        }
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

bool MainWindow::eventFilter(QObject *obj, QEvent *event)
{
    if (obj == m_microcodeEditorWindow && event->type() == QEvent::Close) {
        m_microcodeEditorWindow->hide();
        event->ignore();
        return true;
    }
    else if (obj == m_memoryEditorWindow && event->type() == QEvent::Close) {
        m_memoryEditorWindow->hide();
        event->ignore();
        return true;
    }
    else if (obj == m_instructionEditorWindow && event->type() == QEvent::Close) {
        m_instructionEditorWindow->hide();
        event->ignore();
        return true;
    }
    return QMainWindow::eventFilter(obj, event);
}

