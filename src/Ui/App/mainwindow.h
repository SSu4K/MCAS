#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulationview.h"

#include "executionworker.h"

class MainWindow : public QMainWindow
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

public slots:
    void retranslateUi();
    void open();
    void onSimulationHalted(const QString &reason);

private:
    SimulationView *simView;

    QString err;
    Sim::ExecutionWorker *worker;

    void createToolsMenu();
    void createViewMenu();
    void createMenu();
};

#endif // MAINWINDOW_H
