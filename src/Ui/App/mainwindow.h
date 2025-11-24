#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MicrocodeEditor/microcodeeditorwindow.h"
#include "MemoryEditor/memoryeditorwindow.h"
#include "InstructionEditor/instructioneditorwindow.h"

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

    void openInstructionEditorWindow();
    void closeInstructionEditorWindow();

private:
    MicrocodeEditor::MicrocodeEditorWindow *m_microcodeEditorWindow = nullptr;
    MemoryEditor::MemoryEditorWindow *m_memoryEditorWindow = nullptr;
    InstructionEditor::InstructionEditorWindow * m_instructionEditorWindow = nullptr;

    bool eventFilter(QObject *obj, QEvent *event);
    void createToolsMenu();
    void createViewMenu();
    void createMenu();
    void retranslateUi();

};

#endif // MAINWINDOW_H
