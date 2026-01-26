#ifndef UISUBSYSTEM_H
#define UISUBSYSTEM_H

#include <QObject>

#include "App/mainwindow.h"
#include "MemoryEditor/memoryeditorwindow.h"
#include "MicrocodeEditor/microcodeeditorwindow.h"
#include "InstructionEditor/instructioneditorwindow.h"
#include "Config/configwindow.h"

class ModelsSubsystem;

class UiSubsystem : public QObject
{
    Q_OBJECT
public:
    explicit UiSubsystem(ModelsSubsystem &modelsSubsystem, QObject *parent = nullptr);
    MainWindow mainWindow;

    Ui::MemoryEditorWindow memoryEditorWindow;
    Ui::ConfigWindow configWindow;
    Ui::InstructionEditorWindow instructionEditorWindow;
    Ui::MicrocodeEditorWindow microcodeEditorWindow;
};

#endif // UISUBSYSTEM_H
