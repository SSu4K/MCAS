#ifndef MICROCODEEDITORWINDOW_H
#define MICROCODEEDITORWINDOW_H

#include "uiLib_export.h"

#include <QTabWidget>

#include "Common/editorwindow.h"
#include "microcodeeditorwidget.h"
#include "jumptableeditorwidget.h"

// forward declarations
namespace Models{
    class MicrocodeModel;
    class JumpTableModel;
}

namespace Ui{

    class MicrocodeEditorWidget;
    class JumpTableEditorWidget;

    class UILIB_API MicrocodeEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWindow(Models::MicrocodeModel* microcodeModel, Models::JumpTableModel* jumpTableModel, QWidget* parent = nullptr);

    public slots:
        void open();
        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;

    protected:
        QString windowTitle() const override { return tr("Microcode Editor"); }
        QString openFilePrompt() const override { return tr("Open Microcode File"); }
        QString saveAsFilePrompt() const override { return tr("Save Microcode File As"); }
        QString defaultFilename() const override { return "microcode.mco"; }
        QString fileFilterString() const override { return "Microcode Files (*.mco)"; }

        void clearData() override;

        void createCustomMenu() override;
        void retranslateCustomMenu() override;

    private:
        Models::MicrocodeModel* microcodeModel;
        Models::JumpTableModel* jumpTableModel;

        QTabWidget tabWidget;
        MicrocodeEditorWidget microcodeEditor;
        JumpTableEditorWidget jumpTableEditor;

        int microcodeTabIndex;
        int jumpTableTabIndex;
    };

}

#endif // MICROCODEEDITORWINDOW_H
