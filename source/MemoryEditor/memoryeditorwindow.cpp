#include "memoryeditorwindow.h"

using namespace MemoryEditor;

MemoryEditorWindow::MemoryEditorWindow(QWidget *parent)
    : EditorWindow{parent}
{
    setWindowTitle(windowTitle());
    qDebug() << defaultFilename();
    qDebug() << fileFilterString();

    m_memoryEditorWidget = new MemoryEditorWidget();
    setCentralWidget(m_memoryEditorWidget);
}

bool MemoryEditorWindow::serializeToFile(QFile &file) const {
    // do nothing for now
    return true;
}

bool MemoryEditorWindow::serializeFromFile(QFile &file){
    // do nothing for now
    return true;
}

void MemoryEditorWindow::clearData(){
    // do nothing for now
}
