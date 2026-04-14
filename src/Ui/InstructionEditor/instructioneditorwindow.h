#ifndef INSTRUCTIONEDITORWINDOW_H
#define INSTRUCTIONEDITORWINDOW_H

#include "uiLib_export.h"

#include "Common/editorwindow.h"
#include "instructioneditorwidget.h"
#include "breakpointwidget.h"

// forward declarations
namespace Models {
    class InstructionModel;
    class BreakpointModel;
}

namespace Ui{

    // forward declarations
    class InstructionEditorWidget;

    class UILIB_API  InstructionEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit InstructionEditorWindow(Models::InstructionModel *instructionModel, Models::BreakpointModel *breakpointModel, QWidget *parent = nullptr);

    public slots:
        void open();
        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;

    protected:
        QString windowTitle() const override { return tr("Instruction Editor"); }
        QString openFilePrompt() const override { return tr("Open Instruction File"); }
        QString saveAsFilePrompt() const override { return tr("Save Instruction File As"); }
        QString defaultFilename() const override { return "instr.cod"; }
        QString fileFilterString() const override { return "Memory Files (*.cod)"; }

        void clearData() override;

        void createCustomMenu() override;
        void retranslateCustomMenu() override;

        Models::InstructionModel *instructionModel;
        Models::BreakpointModel *breakpointModel;

        QTabWidget tabWidget;
        InstructionEditorWidget instructionEditor;
        BreakpointWidget breakpointEditor;

        int instructionTabIndex;
        int breakpointTabIndex;

    signals:
    };

}
#endif // INSTRUCTIONEDITORWINDOW_H
