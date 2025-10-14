#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include <QMainWindow>
#include "microcodeeditorwidget.h"
#include "jumptableeditorwidget.h"

namespace MicrocodeEditor{

    class MicrocodeEditorWindow : public QMainWindow
    {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWindow(QWidget* parent = nullptr);

    private slots:
        void newFile();
        void openFile();
        void saveFile();
        void saveFileAs();
        void exitApp();

    private:
        void createMenus();
        bool maybeSave();
        bool saveToFile(const QString& filePath);
        bool loadFromFile(const QString& filePath);

        QTabWidget* m_tabWidget;
        MicrocodeEditorWidget* m_microcodeEditor;
        JumpTableEditorWidget* m_jumpTableEditor;

        QString m_currentFilePath;
        bool m_isModified = false;
    };

}

#endif // MICROCODEEDITORWINDOW_H
