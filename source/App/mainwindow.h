#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QActionGroup>
#include <QEvent>

#include "MicrocodeEditor/microcodeeditorwindow.h"
#include "MemoryEditor/memoryeditorwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;


private slots:
    void openMicrocodeEditorWindow();
    void closeMicrocodeEditorWindow();

    void openMemoryEditorWindow();
    void closeMemoryEditorWindow();

private:
    MicrocodeEditor::MicrocodeEditorWindow *m_microcodeEditorWindow = nullptr;
    MemoryEditor::MemoryEditorWindow *m_memoryEditorWindow = nullptr;

    bool eventFilter(QObject *obj, QEvent *event);
    void createToolsMenu();
    void createViewMenu();
    void createMenu();
    void retranslateUi();

};

#endif // MAINWINDOW_H
