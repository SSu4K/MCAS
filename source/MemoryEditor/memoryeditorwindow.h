#ifndef MEMORYEDITORWINDOW_H
#define MEMORYEDITORWINDOW_H

#include <QMainWindow>
#include "Common/editorwindow.h"

namespace MemoryEditor{

    class MemoryEditorWindow : public EditorWindow
    {
        Q_OBJECT
    public:
        explicit MemoryEditorWindow(QWidget *parent = nullptr);

    protected:
        QString windowTitle() const override { return "Memory Editor"; }
        QString openFilePrompt() const override { return "Open Memory File"; }
        QString saveAsFilePrompt() const override { return "Save Memory File As"; }
        QString defaultFilename() const override { return "memory.txt"; }
        QString fileFilterString() const override { return "Memory Files (*.cpp *.txt)"; }

        bool serializeToStream(QTextStream &stream) const override;
        bool serializeFromStream(QTextStream const &stream) override;

    signals:
    };
}
#endif // MEMORYEDITORWINDOW_H
