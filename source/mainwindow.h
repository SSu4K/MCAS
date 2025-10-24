#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QActionGroup>
#include <QEvent>

#include "MicrocodeEditor/microcodeeditorwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;


private slots:
    void openMicrocodeEditorWindow();
    void closeMicrocodeEditorWindow();

private:
    MicrocodeEditor::MicrocodeEditorWindow *m_microcodeEditorWindow = nullptr;

    bool eventFilter(QObject *obj, QEvent *event);
    void createToolsMenu();
    void createViewMenu();
};

#endif // MAINWINDOW_H
