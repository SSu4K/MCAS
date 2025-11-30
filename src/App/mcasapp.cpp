#include <QStyleFactory>
#include <QStyle>

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
    : memoryModel(context.sharedData()->editorMachineState()),
    instructionModel(context.sharedData()->editorMachineState(),
                       context.sharedData()->labels(),
                       context.sharedData()->instructionSet(),
                       context.sharedData()->instructions(),
                       ),
    microcodeModel(context.sharedData()->microcode()),
    jumpTableModel(context.sharedData()->jumptable()),

    mainWindow(),
    microcodeEditorWindow(&microcodeModel, &jumpTableModel),
    memoryEditorWindow(&memoryModel),
    instructionEditorWindow(&instructionModel),

    QApplication(argc, argv)
{
    setWindowIcon(QIcon(":/icons/appicon.png"));

    QFile f(":/icons/appicon.png");
    qDebug() << "Icon exists?" << f.exists();

    setOrganizationName("SzymonSudak");
    setApplicationName("MCAS");
    setApplicationVersion(context.appVersion());

    setStyle(QStyleFactory::create("Fusion"));
    initPalette();
    initTranslations();

    // conncect coupled models
    connect(&memoryModel, &MemoryModel::memoryRegionChanged, &instructionModel, &InstructionModel::onMemoryRegionChanged);
    connect(&instructionModel, &InstructionModel::memoryRegionChanged, &memoryModel, &MemoryModel::onMemoryRegionChanged);

    // connect open windows
    connect(&mainWindow, &MainWindow::openMicrocodeEditorWindow,
            &microcodeEditorWindow, &MicrocodeEditorWindow::open);

    connect(&mainWindow, &MainWindow::openMemoryEditorWindow,
            &memoryEditorWindow, &MemoryEditorWindow::open);

    connect(&mainWindow, &MainWindow::openInstructionEditorWindow,
            &instructionEditorWindow, &InstructionEditorWindow::open);

    // connect retranslations
    connect(&context, &AppContext::languageChanged,
            &mainWindow, &MainWindow::retranslateUi);
    connect(&context, &AppContext::languageChanged,
            &microcodeEditorWindow, &MicrocodeEditorWindow::retranslateUi);
    connect(&context, &AppContext::languageChanged,
            &memoryEditorWindow, &MemoryEditorWindow::retranslateUi);
    connect(&context, &AppContext::languageChanged,
            &instructionEditorWindow, &InstructionEditorWindow::retranslateUi);

    mainWindow.open();
}

void MCASApp::initTranslations()
{
    context.initLanguage();
}

void MCASApp::initPalette()
{
    using Theme = AppContext::Theme;
    Theme theme = context.currentTheme();

    if (theme == Theme::System) {
        setPalette(style()->standardPalette());
    }
    else if (theme == Theme::Dark) {
        setPalette(getDarkPalette());
    } else if (theme == Theme::Light) {
        setPalette(getLightPalette());
    }
}

void MCASApp::initMainWindow()
{
}
