#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>

class EditorWindow : public QMainWindow
{
public:
    explicit EditorWindow(QWidget* parent = nullptr);

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

    QString m_currentFilePath;

};

#endif // EDITORWINDOW_H
