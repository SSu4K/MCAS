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
    QFile f(":/icons/appicon.png");
    if(f.exists()){
        setWindowIcon(QIcon(":/icons/appicon.png"));
        qDebug() << "Loading icon success!";
    }
    else{
        qDebug() << "Loading icon failed!";
    }

    setStyle(QStyleFactory::create("Fusion"));
    initSettingsSubsystem();
    initProjectSubsystem();
}

void MCASApp::themeChanged(const QString& theme){
    qDebug() << "app: themeChanged:" << theme;
    if(theme == "System"){
        setPalette(style()->standardPalette());
    }
    else if (theme == "Light"){
        setPalette(getLightPalette());
    }
    else if (theme == "Dark"){
        setPalette(getDarkPalette());
    }

    qDebug() << "Is dark theme?" << qApp->palette().color(QPalette::Window).value();
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
}

void MCASApp::initProjectSubsystem(){
    connect(&ui.mainWindow, &MainWindow::serializeFromFile, &project, &ProjectSubsystem::loadProject);
}
