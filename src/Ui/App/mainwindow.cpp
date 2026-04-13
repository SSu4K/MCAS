#include <QMenuBar>
#include <QActionGroup>
#include <QLayout>
#include <QMessageBox>
#include <QFileDialog>

#include "haltdialog.h"
#include "mainwindow.h"

MainWindow::MainWindow(Sim::ExecutionWorker *worker, QWidget *parent)
    : worker(worker), QMainWindow(parent)
{
    setWindowTitle(windowTitle());
    resize(700, 500);

    createMenu();

    simView = new SimulationView(this);
    setCentralWidget(simView);

    connect(simView, &SimulationView::resetClicked, worker, &Sim::ExecutionWorker::reset);

    connect(simView, &SimulationView::clockClicked, worker, &Sim::ExecutionWorker::stepMicro);
    connect(simView, &SimulationView::rewindClicked, worker, &Sim::ExecutionWorker::rewindMicro);

    connect(simView, &SimulationView::stepInstrClicked, worker, &Sim::ExecutionWorker::stepInstr);
    connect(simView, &SimulationView::rewindInstrClicked, worker, &Sim::ExecutionWorker::rewindInstruction);

    connect(simView, &SimulationView::runClicked, worker, &Sim::ExecutionWorker::run);
    connect(simView, &SimulationView::stopClicked, worker, &Sim::ExecutionWorker::stop);

    connect(worker, &Sim::ExecutionWorker::stateChanged, this, &MainWindow::updateSimView);

    connect(simView, &SimulationView::clockFrequencyChanged,
            worker, &Sim::ExecutionWorker::setFrequency);

    connect(worker, &Sim::ExecutionWorker::halted, this, &MainWindow::onSimulationHalted);
}

void MainWindow::updateSimView(){
    simView->updateUAR(worker->currentUAR());
    simView->updateState(worker->getMachineState());
}

void MainWindow::open()
{
    this->show();
    this->raise();
    this->activateWindow();
}

void MainWindow::createFileMenu(){
    QMenu* fileMenu = menuBar()->addMenu(tr("&File"));

    fileMenu->addAction(tr("&New Project"), QKeySequence::New, this, &MainWindow::newFile);
    fileMenu->addAction(tr("&Open Project"), QKeySequence::Open, this, &MainWindow::openFile);

    fileMenu->addSeparator();
    fileMenu->addAction(tr("&Save Project"), QKeySequence::Save, this, &MainWindow::saveFile);
    fileMenu->addAction(tr("Save Project &As..."), QKeySequence::SaveAs, this, &MainWindow::saveFileAs);
}

void MainWindow::createToolsMenu(){
    QMenu *toolsMenu = menuBar()->addMenu(tr("&Tools"));

    QAction *openConfigWindowAction = new QAction(tr("Configure"), this);
    toolsMenu->addAction(openConfigWindowAction);
    connect(openConfigWindowAction, &QAction::triggered, this, &MainWindow::openConfigWindow);

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
    createFileMenu();
    createToolsMenu();
    createViewMenu();
}

void MainWindow::retranslateUi(){
    setWindowTitle(tr("MCAS Main Window"));
    menuBar()->clear();
    createMenu();
    simView->retranslateUI();
}

void MainWindow::onSimulationHalted(const QString &reason)
{
    // HaltDialog dlg(this);
    // dlg.setReason(reason);
    // dlg.exec();
    QMessageBox::warning(
        this,
        "Simulation Halted",
        reason,
        QMessageBox::Ok
        );
}

void MainWindow::newFile()
{
    if (!maybeSave())
        return;

    m_currentFilePath.clear();
    setWindowTitle(windowTitle() + " - [New]");

    emit newProject();
}

void MainWindow::openFile()
{
    if (!maybeSave())
        return;

    QString filePath = QFileDialog::getOpenFileName(this, "Open Project File",
                                                    "", "*.txt *.proj");
    if (filePath.isEmpty()){
        qDebug("Empty path!");
        return;
    }

    QFile file(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)){
        qDebug("Failed to open!");
        return;
    }

    qDebug() << "Serializing priject from" << filePath;

    m_currentFilePath = filePath;
    setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(filePath).fileName()));

    emit serializeFromFile(file);
}

void MainWindow::saveFile()
{
    if (m_currentFilePath.isEmpty()) {
        saveFileAs();
        return;
    }

    QFile file(m_currentFilePath);

    qDebug() << "Serializing to project" << m_currentFilePath;
    emit serializeToFile(file);
}

void MainWindow::saveFileAs()
{
    QString filePath = QFileDialog::getSaveFileName(this, "Save Project File As",
                                                    m_currentFilePath.isEmpty() ? "project.proj" : m_currentFilePath,
                                                    "*.txt *.proj");
    if (filePath.isEmpty())
        return;

    QFile file(filePath);

    bool success = true;
    success = serializeToFile(file);

    qDebug() << "Serializing to" << m_currentFilePath << "Success:" << success;

    if (success && m_currentFilePath.isEmpty()) {
        m_currentFilePath = filePath;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

bool MainWindow::maybeSave()
{
    return true; // simplify for now
}

