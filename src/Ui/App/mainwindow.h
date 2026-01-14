#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "simulationview.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

signals:
    void openMicrocodeEditorWindow();
    void openMemoryEditorWindow();
    void openInstructionEditorWindow();

    void setTheme(QString theme);
    void setLanguage(QString language);

public slots:
    void retranslateUi();
    void open();

private:
    SimulationView *simView;

    void createToolsMenu();
    void createViewMenu();
    void createMenu();
};

#endif // MAINWINDOW_H
