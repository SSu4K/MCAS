#ifndef MCASAPP_H
#define MCASAPP_H

#include <QApplication>
#include <QIcon>

#include "settingssubsystem.h"
#include "simulationsubsystem.h"
#include "modelssubsystem.h"
#include "uisubsystem.h"
// #include "appcontext.h"

// #include "Memory/memorymodel.h"
// #include "Instruction/instructionmodel.h"
// #include "Microcode/microcodemodel.h"
// #include "Microcode/jumptablemodel.h"

// #include "App/mainwindow.h"
// #include "MemoryEditor/memoryeditorwindow.h"
// #include "MicrocodeEditor/microcodeeditorwindow.h"
// #include "InstructionEditor/instructioneditorwindow.h"

// forward declaration
class AppContext;
class MainWindow;

class MCASApp : public QApplication
{
    Q_OBJECT
public:
    MCASApp(int &argc, char **argv);
    ~MCASApp() override = default;


private:

    SettingsSubsystem settings;
    SimulationSubsystem simulation;
    ModelsSubsystem models;
    UiSubsystem ui;

    QTranslator translator;

    // App-wide setup
    void initSettingsSubsystem();
    void loadLanguage(const QString& lang);

public slots:
    void themeChanged(const QString&);

signals:
    void languageChanged();
};
#endif // MCASAPP_H
