#ifndef INSTRUCTIONEDITORWINDOW_H
#define INSTRUCTIONEDITORWINDOW_H

#include "Common/editorwindow.h"
#include "instructioneditorwidget.h"

// forward declarations
namespace Models {
    class InstructionModel;
}

namespace Ui{

    // forward declarations
    class InstructionEditorWidget;

    class InstructionEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit InstructionEditorWindow(Models::InstructionModel *model, QWidget *parent = nullptr);

    public slots:
        void open();

    protected:
        QString windowTitle() const override { return tr("Instruction Editor"); }
        QString openFilePrompt() const override { return tr("Open Instruction File"); }
        QString saveAsFilePrompt() const override { return tr("Save Instruction File As"); }
        QString defaultFilename() const override { return "instr.cod"; }
        QString fileFilterString() const override { return "Memory Files (*.cod)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

        void createCustomMenu() override;

        Models::InstructionModel *model;
        InstructionEditorWidget widget;

    signals:
    };

}
#endif // INSTRUCTIONEDITORWINDOW_H
