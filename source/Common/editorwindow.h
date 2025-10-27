#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QFile>

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

protected:
    // Child classes override these
    virtual QString windowTitle() const { return "[Type] Editor"; }
    virtual QString openFilePrompt() const { return "Open [Type] File"; }
    virtual QString saveAsFilePrompt() const { return "Save [Type] File As"; }
    virtual QString defaultFilename() const { return "type.txt"; }
    virtual QString fileFilterString() const { return "[Type] Files (*.txt *.mc)"; }

    virtual bool serializeToFile(QFile& filePath) const {return true;}
    virtual bool serializeFromFile(QFile& filePath) {return true;}
    virtual void clearData() {}

private:
    QString m_currentFilePath;

    void createMenus();
    bool maybeSave();
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
};

#endif // EDITORWINDOW_H
