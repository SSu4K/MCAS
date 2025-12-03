#include "uisubsystem.h"
#include "modelssubsystem.h"

using namespace Ui;

UiSubsystem::UiSubsystem(ModelsSubsystem &modelsSubsystem, QObject *parent)
    : QObject{parent},
    mainWindow(),
    microcodeEditorWindow(&modelsSubsystem.microcodeModel, &modelsSubsystem.jumpTableModel),
    memoryEditorWindow(&modelsSubsystem.memoryModel),
    instructionEditorWindow(&modelsSubsystem.instructionModel)
{
    connect(&mainWindow, &MainWindow::openMicrocodeEditorWindow,
            &microcodeEditorWindow, &MicrocodeEditorWindow::open);

    connect(&mainWindow, &MainWindow::openMemoryEditorWindow,
            &memoryEditorWindow, &MemoryEditorWindow::open);

    connect(&mainWindow, &MainWindow::openInstructionEditorWindow,
            &instructionEditorWindow, &InstructionEditorWindow::open);

    mainWindow.open();
}
