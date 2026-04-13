#include "uisubsystem.h"
#include "modelssubsystem.h"

using namespace Ui;

UiSubsystem::UiSubsystem(ModelsSubsystem &modelsSubsystem, QObject *parent)
    : QObject{parent},
    mainWindow(&modelsSubsystem.simulation.worker),
    microcodeEditorWindow(&modelsSubsystem.microcodeModel, &modelsSubsystem.jumpTableModel, &mainWindow),
    memoryEditorWindow(&modelsSubsystem.memoryModel, &mainWindow),
    configWindow(&modelsSubsystem.instructionSetModel, &mainWindow),
    instructionEditorWindow(&modelsSubsystem.instructionModel, &modelsSubsystem.breakpointsModel, &mainWindow)
{
    connect(&mainWindow, &MainWindow::openConfigWindow,
            &configWindow, &ConfigWindow::open);

    connect(&mainWindow, &MainWindow::openMicrocodeEditorWindow,
            &microcodeEditorWindow, &MicrocodeEditorWindow::open);

    connect(&mainWindow, &MainWindow::openMemoryEditorWindow,
            &memoryEditorWindow, &MemoryEditorWindow::open);

    connect(&mainWindow, &MainWindow::openInstructionEditorWindow,
            &instructionEditorWindow, &InstructionEditorWindow::open);

    mainWindow.open();
}
