#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include <QMainWindow>
#include <QFile>

static const char* FILE_MENU_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&File");
static const char* NEW_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&New");
static const char* OPEN_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&Open...");
static const char* SAVE_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&Save");
static const char* SAVE_AS_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "Save &As...");
static const char* EXIT_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "E&xit");

class EditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget* parent = nullptr);

private slots:
    void newFile();
    void openFile();
    void saveFile();
    void saveFileAs();
    void exitApp();

protected:
    void createMenu();
    void retranslateUi();

    // Child classes override these
    virtual QString windowTitle() const { return "[Type] Editor"; }
    virtual QString openFilePrompt() const { return "Open [Type] File"; }
    virtual QString saveAsFilePrompt() const { return "Save [Type] File As"; }
    virtual QString defaultFilename() const { return "type.txt"; }
    virtual QString fileFilterString() const { return "[Type] Files (*.txt *.mc)"; }

    virtual bool serializeToFile(QFile& filePath) const {return true;}
    virtual bool serializeFromFile(QFile& filePath) {return true;}
    virtual void clearData() {}

    virtual void createCustomMenu() {}

private:
    QString m_currentFilePath;

    bool maybeSave();
    bool saveToFile(const QString& filePath);
    bool loadFromFile(const QString& filePath);
};

#endif // EDITORWINDOW_H
