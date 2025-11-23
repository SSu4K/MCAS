#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include "Common/editorwindow.h"

namespace MicrocodeEditor{
    // forward declarations
    class MicrocodeEditorWidget;
    class JumpTableEditorWidget;

    class MicrocodeEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWindow(QWidget* parent = nullptr);

    protected:
        QString windowTitle() const override { return tr("Microcode Editor"); }
        QString openFilePrompt() const override { return tr("Open Microcode File"); }
        QString saveAsFilePrompt() const override { return tr("Save Microcode File As"); }
        QString defaultFilename() const override { return "microcode.txt"; }
        QString fileFilterString() const override { return "Microcode Files (*.mco *.txt)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

        void createCustomMenu() override;

    private:

        QTabWidget* m_tabWidget;
        MicrocodeEditorWidget* m_microcodeEditor;
        JumpTableEditorWidget* m_jumpTableEditor;
    };

}

#endif // MICROCODEEDITORWINDOW_H
