#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "MicrocodeEditor/microcodeeditorwindow.h"
#include <QMainWindow>

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
};

#endif // MAINWINDOW_H
