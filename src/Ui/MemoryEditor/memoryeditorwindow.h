#ifndef MEMORYEDITORWINDOW_H
#define MEMORYEDITORWINDOW_H

#include "Common/editorwindow.h"
#include "memoryeditorwidget.h"

namespace MemoryEditor{

    class MemoryModel;

    class MemoryEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MemoryEditorWindow(QWidget *parent = nullptr);
        MemoryModel* getModel();

    protected:
        QString windowTitle() const override { return tr("Memory Editor"); }
        QString openFilePrompt() const override { return tr("Open Memory File"); }
        QString saveAsFilePrompt() const override { return tr("Save Memory File As"); }
        QString defaultFilename() const override { return "memory.txt"; }
        QString fileFilterString() const override { return "Memory Files (*.dat *.txt)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

        void createCustomMenu() override;

    private:
        MemoryEditorWidget *m_memoryEditorWidget;
    };
}
#endif // MEMORYEDITORWINDOW_H
