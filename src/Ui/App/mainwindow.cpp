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

void MainWindow::createFileMenu()
{
    m_fileMenu = menuBar()->addMenu("");

    m_newProjectAction = m_fileMenu->addAction("");
    m_newProjectAction->setShortcut(QKeySequence::New);
    connect(m_newProjectAction, &QAction::triggered, this, &MainWindow::newFile);

    m_openProjectAction = m_fileMenu->addAction("");
    m_openProjectAction->setShortcut(QKeySequence::Open);
    connect(m_openProjectAction, &QAction::triggered, this, &MainWindow::openFile);

    m_fileMenu->addSeparator();

    m_saveProjectAction = m_fileMenu->addAction("");
    m_saveProjectAction->setShortcut(QKeySequence::Save);
    connect(m_saveProjectAction, &QAction::triggered, this, &MainWindow::saveFile);

    m_saveAsProjectAction = m_fileMenu->addAction("");
    m_saveAsProjectAction->setShortcut(QKeySequence::SaveAs);
    connect(m_saveAsProjectAction, &QAction::triggered, this, &MainWindow::saveFileAs);
}

void MainWindow::createToolsMenu()
{
    m_toolsMenu = menuBar()->addMenu("");

    m_configAction = m_toolsMenu->addAction("");
    connect(m_configAction, &QAction::triggered, this, &MainWindow::openConfigWindow);

    m_microcodeEditorAction = m_toolsMenu->addAction("");
    connect(m_microcodeEditorAction, &QAction::triggered, this, &MainWindow::openMicrocodeEditorWindow);

    m_memoryEditorAction = m_toolsMenu->addAction("");
    connect(m_memoryEditorAction, &QAction::triggered, this, &MainWindow::openMemoryEditorWindow);

    m_instructionEditorAction = m_toolsMenu->addAction("");
    connect(m_instructionEditorAction, &QAction::triggered, this, &MainWindow::openInstructionEditorWindow);
}

void MainWindow::createViewMenu()
{
    m_viewMenu = menuBar()->addMenu("");
    m_themeMenu = m_viewMenu->addMenu("");

    m_systemThemeAction = m_themeMenu->addAction("");
    m_lightThemeAction  = m_themeMenu->addAction("");
    m_darkThemeAction   = m_themeMenu->addAction("");

    m_systemThemeAction->setCheckable(true);
    m_lightThemeAction->setCheckable(true);
    m_darkThemeAction->setCheckable(true);

    m_themeGroup = new QActionGroup(this);
    m_themeGroup->addAction(m_systemThemeAction);
    m_themeGroup->addAction(m_lightThemeAction);
    m_themeGroup->addAction(m_darkThemeAction);
    m_themeGroup->setExclusive(true);

    connect(m_themeGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        if (action == m_systemThemeAction) emit setTheme("System");
        if (action == m_lightThemeAction)  emit setTheme("Light");
        if (action == m_darkThemeAction)   emit setTheme("Dark");
    });

    // --- Language ---
    m_languageMenu = m_viewMenu->addMenu("");

    m_langGroup = new QActionGroup(this);

    m_enAction = m_languageMenu->addAction("");
    m_enAction->setCheckable(true);

    m_plAction = m_languageMenu->addAction("");
    m_plAction->setCheckable(true);

    m_langGroup->addAction(m_enAction);
    m_langGroup->addAction(m_plAction);
    m_langGroup->setExclusive(true);

    connect(m_langGroup, &QActionGroup::triggered, this, [this](QAction *action) {
        if (action == m_enAction){
            emit setLanguage("en");
        }
        if (action == m_plAction){
            emit setLanguage("pl");
        }
    });
}

void MainWindow::createMenu(){
    createFileMenu();
    createToolsMenu();
    createViewMenu();
}

void MainWindow::retranslateUi()
{
    setWindowTitle(tr("MCAS")); // or whatever is correct

    // --- File ---
    m_fileMenu->setTitle(tr("&File"));
    m_newProjectAction->setText(tr("&New Project"));
    m_openProjectAction->setText(tr("&Open Project"));
    m_saveProjectAction->setText(tr("&Save Project"));
    m_saveAsProjectAction->setText(tr("Save Project &As..."));

    // --- Tools ---
    m_toolsMenu->setTitle(tr("&Tools"));
    m_configAction->setText(tr("Configure"));
    m_microcodeEditorAction->setText(tr("Microcode Editor"));
    m_memoryEditorAction->setText(tr("Memory Editor"));
    m_instructionEditorAction->setText(tr("Instruction Editor"));

    // --- View ---
    m_viewMenu->setTitle(tr("&View"));
    m_themeMenu->setTitle(tr("Theme"));

    m_systemThemeAction->setText(tr("System Default"));
    m_lightThemeAction->setText(tr("Light"));
    m_darkThemeAction->setText(tr("Dark"));

    m_languageMenu->setTitle(tr("Language"));
    m_enAction->setText(tr("English"));
    m_plAction->setText(tr("Polish"));

    // propagate
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
    m_currentFilePath.clear();
    setWindowTitle(windowTitle() + " - [New]");

    emit newProject();
}

void MainWindow::openFile()
{

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

    qDebug() << "Serializing project from:" << filePath;

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

    qDebug() << "Serializing to project:" << m_currentFilePath;
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

    qDebug() << "Serializing to:" << m_currentFilePath << "Success:" << success;

    if (success && m_currentFilePath.isEmpty()) {
        m_currentFilePath = filePath;
        setWindowTitle(QString(windowTitle() + " - [%1]").arg(QFileInfo(filePath).fileName()));
    }
}

