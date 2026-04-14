#include <QStyleFactory>
#include <QStyle>
#include <QFile>

#include "mcasapp.h"

using namespace Models;
using namespace Ui;

static QPalette getDarkPalette(){
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(45, 45, 45));
    palette.setColor(QPalette::WindowText, Qt::white);
    palette.setColor(QPalette::Base, QColor(30, 30, 30));
    palette.setColor(QPalette::AlternateBase, QColor(45, 45, 45));
    palette.setColor(QPalette::ToolTipBase, Qt::white);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::white);
    palette.setColor(QPalette::Button, QColor(45, 45, 45));
    palette.setColor(QPalette::ButtonText, Qt::white);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    return palette;
}

static QPalette getLightPalette(){
    QPalette palette;
    palette.setColor(QPalette::Window, QColor(250, 250, 250));
    palette.setColor(QPalette::WindowText, Qt::black);
    palette.setColor(QPalette::Base, QColor(255, 255, 255));
    palette.setColor(QPalette::AlternateBase, QColor(245, 245, 245));
    palette.setColor(QPalette::ToolTipBase, Qt::black);
    palette.setColor(QPalette::ToolTipText, Qt::white);
    palette.setColor(QPalette::Text, Qt::black);
    palette.setColor(QPalette::Button, QColor(240, 240, 240));
    palette.setColor(QPalette::ButtonText, Qt::black);
    palette.setColor(QPalette::BrightText, Qt::red);
    palette.setColor(QPalette::Highlight, QColor(76, 163, 224));
    palette.setColor(QPalette::HighlightedText, Qt::white);
    return palette;
}

MCASApp::MCASApp(int &argc, char **argv)
    : settings(), simulation(), models(simulation), project(models), ui(models),
    QApplication(argc, argv)
{
    setStyle(QStyleFactory::create("Fusion"));
    initSettingsSubsystem();
    initProjectSubsystem();
}

void MCASApp::themeChanged(const QString& theme){
    if(theme == "System"){
        setPalette(style()->standardPalette());
    }
    else if (theme == "Light"){
        setPalette(getLightPalette());
    }
    else if (theme == "Dark"){
        setPalette(getDarkPalette());
    }
}


void MCASApp::loadLanguage(const QString& lang)
{
    removeTranslator(&translator);
    QString path = QString(":/i18n/MCAS_%1.qm").arg(lang);

    if (!translator.load(path))
    {
        qWarning() << "Failed to load translator" << path;
        return;
    }

    installTranslator(&translator);

    emit languageChanged();
}

void MCASApp::initSettingsSubsystem(){
    setOrganizationName("SzymonSudak");
    setApplicationName("MCAS");

    setStyle(QStyleFactory::create("Fusion"));

    // connect theme change
    connect(&ui.mainWindow, &MainWindow::setTheme, &settings, &SettingsSubsystem::setTheme);
    connect(&settings, &SettingsSubsystem::themeChanged, this, &MCASApp::themeChanged);

    // ensure initial theme is correct
    themeChanged(settings.currentTheme());

    // connect language change
    connect(&ui.mainWindow, &MainWindow::setLanguage, &settings, &SettingsSubsystem::setLanguage);
    connect(&settings, &SettingsSubsystem::languageChanged,
            this, &MCASApp::loadLanguage);

    // connect window retranslations
    connect(this, &MCASApp::languageChanged,
            &ui.mainWindow, &MainWindow::retranslateUi);
    connect(this, &MCASApp::languageChanged,
            &ui.microcodeEditorWindow, &MicrocodeEditorWindow::retranslateUi);
    connect(this, &MCASApp::languageChanged,
            &ui.memoryEditorWindow, &MemoryEditorWindow::retranslateUi);
    connect(this, &MCASApp::languageChanged,
            &ui.instructionEditorWindow, &InstructionEditorWindow::retranslateUi);
    connect(this, &MCASApp::languageChanged,
            &ui.configWindow, &ConfigWindow::retranslateUi);

    auto initLanguage = settings.language();
    loadLanguage(initLanguage);
}

void MCASApp::initProjectSubsystem(){
    // connect main window buttons
    connect(&ui.mainWindow, &MainWindow::newProject, &project, &ProjectSubsystem::newProject);
    connect(&ui.mainWindow, &MainWindow::serializeFromFile, &project, &ProjectSubsystem::loadProject);
    connect(&ui.mainWindow, &MainWindow::serializeToFile, &project, &ProjectSubsystem::saveProject);

    // connect project subsystem clear to editor windows
    connect(&project, &ProjectSubsystem::clearProject,          &ui.configWindow,               &ConfigWindow::newFile);
    connect(&project, &ProjectSubsystem::clearProject,          &ui.instructionEditorWindow,    &InstructionEditorWindow::newFile);
    connect(&project, &ProjectSubsystem::clearProject,          &ui.memoryEditorWindow,         &MemoryEditorWindow::newFile);
    connect(&project, &ProjectSubsystem::clearProject,          &ui.microcodeEditorWindow,      &MicrocodeEditorWindow::newFile);

    // connect project subsystem load to editor windows
    connect(&project, &ProjectSubsystem::loadConfigFile,        &ui.configWindow,               &ConfigWindow::openFileFromPath);
    connect(&project, &ProjectSubsystem::loadInstructionFile,   &ui.instructionEditorWindow,    &InstructionEditorWindow::openFileFromPath);
    connect(&project, &ProjectSubsystem::loadMemoryFile,        &ui.memoryEditorWindow,         &MemoryEditorWindow::openFileFromPath);
    connect(&project, &ProjectSubsystem::loadMicrocodeFile,     &ui.microcodeEditorWindow,      &MicrocodeEditorWindow::openFileFromPath);

    // connect project subsystem save to editor windows
    connect(&project, &ProjectSubsystem::saveConfigFile,        &ui.configWindow,               &ConfigWindow::saveFileToPath);
    connect(&project, &ProjectSubsystem::saveInstructionFile,   &ui.instructionEditorWindow,    &InstructionEditorWindow::saveFileToPath);
    connect(&project, &ProjectSubsystem::saveMemoryFile,        &ui.memoryEditorWindow,         &MemoryEditorWindow::saveFileToPath);
    connect(&project, &ProjectSubsystem::saveMicrocodeFile,     &ui.microcodeEditorWindow,      &MicrocodeEditorWindow::saveFileToPath);

    // connect editor windows file changes to project subsystem
    connect(&ui.configWindow,               &ConfigWindow::fileChanged,             &project, &ProjectSubsystem::configFileChanged);
    connect(&ui.instructionEditorWindow,    &InstructionEditorWindow::fileChanged,  &project, &ProjectSubsystem::instructionFileChanged);
    connect(&ui.memoryEditorWindow,         &MemoryEditorWindow::fileChanged,       &project, &ProjectSubsystem::memoryFileChanged);
    connect(&ui.microcodeEditorWindow,      &MicrocodeEditorWindow::fileChanged,    &project, &ProjectSubsystem::microcodeFileChanged);
}
