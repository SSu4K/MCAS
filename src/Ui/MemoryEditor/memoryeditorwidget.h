#ifndef MEMORYEDITORWIDGET_H
#define MEMORYEDITORWIDGET_H

#include <QWidget>
#include "Memory/memorymodel.h"

// forward declarations
class QTableView;
class HexIntDelegate;

namespace MemoryEditor {

    // forward declarations
    class MemoryModel;

    class MemoryEditorWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MemoryEditorWidget(QWidget* parent = nullptr);

        MemoryModel *getModel() {return &m_model;}

        MemoryUnitSize getUnitSize();
        void setUnitSize(MemoryUnitSize size);

        void selectAll();
        void clearData();
        void clearSelected();
        void randomSelected();
        void fillSelected();

    protected:
        void resizeEvent(QResizeEvent* event) override;
        bool event(QEvent *e) override;
    private:
        QTableView* m_tableView;
        MemoryModel m_model;
        HexIntDelegate* m_delegate;
        void updateColumnCount();
    };
}

#endif // MEMORYEDITORWIDGET_H
