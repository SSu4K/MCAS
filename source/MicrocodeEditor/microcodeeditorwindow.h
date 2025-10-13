#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include <QMainWindow>
#include "microcodeeditorwidget.h"
#include "zoommainwindow.h"

namespace MicrocodeEditor{

    class MicrocodeEditorWindow : public ZoomMainWindow
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

        MicrocodeEditorWidget* m_editorWidget = nullptr;
        QString m_currentFilePath;
        bool m_isModified = false;
    };

}

#endif // MICROCODEEDITORWINDOW_H
