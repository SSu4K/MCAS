#include "memoryeditorwindow.h"

using namespace MemoryEditor;

MemoryEditorWindow::MemoryEditorWindow(QWidget *parent)
    : EditorWindow{parent}
{
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();
}

bool serializeToStream(QTextStream &stream) {
    // do nothing for now
    return true;
}

bool serializeFromStream(QTextStream const &stream){
    // do nothing for now
    return true;
}
