#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include "Common/editorwindow.h"
#include "microcodeeditorwidget.h"
#include "jumptableeditorwidget.h"

namespace MicrocodeEditor{

    class MicrocodeEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWindow(QWidget* parent = nullptr);

    protected:
        QString windowTitle() const override { return "Microcode Editor"; }
        QString openFilePrompt() const override { return "Open Microcode File"; }
        QString saveAsFilePrompt() const override { return "Save Microcode File As"; }
        QString defaultFilename() const override { return "microcode.txt"; }
        QString fileFilterString() const override { return "Microcode Files (*.mco *.txt)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

    private:

        QTabWidget* m_tabWidget;
        MicrocodeEditorWidget* m_microcodeEditor;
        JumpTableEditorWidget* m_jumpTableEditor;
    };

}

#endif // MICROCODEEDITORWINDOW_H
