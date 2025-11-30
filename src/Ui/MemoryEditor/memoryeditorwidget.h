#ifndef MEMORYEDITORWIDGET_H
#define MEMORYEDITORWIDGET_H

#include <QTableView>
#include <QWidget>

#include "Memory/memorymodel.h"
#include "Common/hexintdelegate.h"

// forward declarations
class QTableView;
class HexIntDelegate;

namespace MemoryEditor {

    // forward declarations
    class MemoryModel;

    class MemoryEditorWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MemoryEditorWidget(MemoryModel* model, QWidget* parent = nullptr);

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
        MemoryModel* model;
        QTableView tableView;
        HexIntDelegate delegate;
        void updateColumnCount();
    };
}

#endif // MEMORYEDITORWIDGET_H
