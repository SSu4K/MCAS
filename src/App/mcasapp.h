#ifndef MCASAPP_H
#define MCASAPP_H

#include <QApplication>
#include <QIcon>

#include "appcontext.h"
#include "App/mainwindow.h"

#include "Memory/memorymodel.h"
#include "Instruction/instructionmodel.h"
#include "Microcode/microcodemodel.h"
#include "Microcode/jumptablemodel.h"

#include "MemoryEditor/memoryeditorwindow.h"
#include "MicrocodeEditor/microcodeeditorwindow.h"
#include "InstructionEditor/instructioneditorwindow.h"

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
    AppContext context;
    QTranslator translator;

    // models
    Models::MemoryModel memoryModel;
    Models::InstructionModel instructionModel;
    Models::MicrocodeModel microcodeModel;
    Models::JumpTableModel jumpTableModel;

    MainWindow mainWindow;
    Ui::MemoryEditorWindow memoryEditorWindow;
    Ui::InstructionEditorWindow instructionEditorWindow;
    Ui::MicrocodeEditorWindow microcodeEditorWindow;

    // App-wide setup
    void initTranslations();
    void initPalette();
    void initMainWindow();
};
#endif // MCASAPP_H
