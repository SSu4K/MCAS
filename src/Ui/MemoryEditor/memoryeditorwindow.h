#ifndef MEMORYEDITORWINDOW_H
#define MEMORYEDITORWINDOW_H

#include "uiLib_export.h"

#include "Common/editorwindow.h"
#include "memoryeditorwidget.h"

namespace Models {
    class MemoryModel;
}

namespace Ui{

    class UILIB_API MemoryEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MemoryEditorWindow(Models::MemoryModel* model, QWidget *parent = nullptr);

    public slots:
        void open();

    protected:
        QString windowTitle() const override { return tr("Memory Editor"); }
        QString openFilePrompt() const override { return tr("Open Memory File"); }
        QString saveAsFilePrompt() const override { return tr("Save Memory File As"); }
        QString defaultFilename() const override { return "memory.dat"; }
        QString fileFilterString() const override { return "Memory Files (*.dat)"; }

        bool serializeToFile(QFile& file) const override;
        bool serializeFromFile(QFile& file) override;
        void clearData() override;

        void createCustomMenu() override;

    private:
        Models::MemoryModel* model;
        MemoryEditorWidget widget;
    };
}
#endif // MEMORYEDITORWINDOW_H
