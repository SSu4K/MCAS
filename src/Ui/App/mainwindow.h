#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "uiLib_export.h"

#include <QMainWindow>
#include "simulationview.h"
#include "executionworker.h"

class UILIB_API MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(Sim::ExecutionWorker *worker, QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void openMicrocodeEditorWindow();
    void openMemoryEditorWindow();
    void openInstructionEditorWindow();
    void openConfigWindow();

    void setTheme(QString theme);
    void setLanguage(QString language);

    void newProject();
    bool serializeToFile(QFile& filePath);
    bool serializeFromFile(QFile& filePath);

public slots:
    void retranslateUi();
    void open();
    void onSimulationHalted(const QString &reason);

private slots:
    void updateSimView();

    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();

private:
    SimulationView *simView;

    QString err;

    Sim::ExecutionWorker *worker;
    QString m_currentFilePath;

    void createFileMenu();
    void createToolsMenu();
    void createViewMenu();
    void createMenu();

    QString windowTitle() const { return tr("MCAS"); }

    // Menus
    QMenu* m_fileMenu;
    QMenu* m_toolsMenu;
    QMenu* m_viewMenu;
    QMenu* m_themeMenu;
    QMenu* m_languageMenu;

    // File actions
    QAction* m_newProjectAction;
    QAction* m_openProjectAction;
    QAction* m_saveProjectAction;
    QAction* m_saveAsProjectAction;

    // Tools actions
    QAction* m_configAction;
    QAction* m_microcodeEditorAction;
    QAction* m_memoryEditorAction;
    QAction* m_instructionEditorAction;

    // Theme actions
    QAction* m_systemThemeAction;
    QAction* m_lightThemeAction;
    QAction* m_darkThemeAction;
    QActionGroup* m_themeGroup;

    // Language actions
    QAction* m_enAction;
    QAction* m_plAction;
    QActionGroup* m_langGroup;

};

#endif // MAINWINDOW_H
