#ifndef INSTRUCTIONEDITORWINDOW_H
#define INSTRUCTIONEDITORWINDOW_H

#include "Common/editorwindow.h"

namespace InstructionEditor{

    // forward declarations
    class InstructionEditorWidget;

    class InstructionEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit InstructionEditorWindow(QWidget *parent = nullptr);

    protected:
        QString windowTitle() const override { return tr("Instruction Editor"); }
        QString openFilePrompt() const override { return tr("Open Instruction File"); }
        QString saveAsFilePrompt() const override { return tr("Save Instruction File As"); }
        QString defaultFilename() const override { return "instr.txt"; }
        QString fileFilterString() const override { return "Memory Files (*.dat *.txt)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

        void createCustomMenu() override;

        InstructionEditorWidget *m_widget;

    signals:
    };

}
#endif // INSTRUCTIONEDITORWINDOW_H
