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

    bool maybeSave();

};

#endif // MAINWINDOW_H
