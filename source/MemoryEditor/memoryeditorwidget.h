#ifndef MEMORYEDITORWIDGET_H
#define MEMORYEDITORWIDGET_H

#include <QTableView>
#include <QResizeEvent>
#include <QHeaderView>

#include "memorymodel.h"

namespace MemoryEditor {

    class MemoryEditorWidget : public QTableView {
        Q_OBJECT
    public:
        explicit MemoryEditorWidget(QWidget* parent = nullptr);
        void setMemory(const QByteArray& memory);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        MemoryModel* m_model;
    };
} // namespace MemoryEditor

#endif // MEMORYEDITORWIDGET_H
