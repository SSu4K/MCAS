#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include <QTabWidget>

#include "Common/editorwindow.h"
#include "microcodeeditorwidget.h"
#include "jumptableeditorwidget.h"

namespace MicrocodeEditor{
    // forward declarations
    class MicrocodeModel;
    class JumpTableModel;

    class MicrocodeEditorWidget;
    class JumpTableEditorWidget;

    class MicrocodeEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWindow(MicrocodeModel* microcodeModel, JumpTableModel* jumpTableModel, QWidget* parent = nullptr);

    public slots:
        void open();

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
        MicrocodeModel* microcodeModel;
        JumpTableModel* jumpTableModel;

        QTabWidget tabWidget;
        MicrocodeEditorWidget microcodeEditor;
        JumpTableEditorWidget jumpTableEditor;
    };

}

#endif // MICROCODEEDITORWINDOW_H
