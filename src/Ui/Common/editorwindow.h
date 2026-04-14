#ifndef EDITORWINDOW_H
#define EDITORWINDOW_H

#include "uiLib_export.h"

#include <QMainWindow>

static const char* FILE_MENU_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&File");
static const char* NEW_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&New");
static const char* OPEN_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&Open...");
static const char* SAVE_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "&Save");
static const char* SAVE_AS_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "Save &As...");
static const char* EXIT_TEXT = QT_TRANSLATE_NOOP("EditorWindow", "E&xit");

class UILIB_API EditorWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit EditorWindow(QWidget* parent = nullptr);

private slots:
    void openFile();
    void saveFile();
    void saveFileAs();
    void exitApp();

public slots:
    void retranslateUi();
    void newFile();
    bool openFileFromPath(const QString &path);
    void saveFileToPath(const QString &path);

signals:
    void fileChanged(const QString &path);

protected:
    void createMenu();

    // Child classes override these
    virtual QString windowTitle() const { return "[Type] Editor"; }
    virtual QString openFilePrompt() const { return "Open [Type] File"; }
    virtual QString saveAsFilePrompt() const { return "Save [Type] File As"; }
    virtual QString defaultFilename() const { return "type.txt"; }
    virtual QString fileFilterString() const { return "[Type] Files (*.txt *.mc)"; }

    virtual void createCustomMenu() {}
    virtual void retranslateCustomMenu() {}

    void updateWindowTitle();

    QMenu* m_fileMenu;

    QAction* m_newAction;
    QAction* m_openAction;
    QAction* m_saveAction;
    QAction* m_saveAsAction;
    QAction* m_exitAction;

public:
    virtual bool serializeToFile(QFile& filePath) const {return true;}
    virtual bool serializeFromFile(QFile& filePath) {return true;}
    virtual void clearData() {}

protected:
    QString m_currentFilePath = "";
};

#endif // EDITORWINDOW_H
