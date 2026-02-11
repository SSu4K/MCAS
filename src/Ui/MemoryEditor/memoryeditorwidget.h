#ifndef MEMORYEDITORWIDGET_H
#define MEMORYEDITORWIDGET_H

#include "uiLib_export.h"

#include <QTableView>
#include <QWidget>

#include "Common/hexintdelegate.h"

// forward declarations
class QTableView;
class HexIntDelegate;

namespace Machine {
enum class MemoryUnitSize;
}

namespace Models {
    class MemoryModel;
}

namespace Ui {

    class UILIB_API MemoryEditorWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MemoryEditorWidget(Models::MemoryModel* model, QWidget* parent = nullptr);

        Machine::MemoryUnitSize getUnitSize();
        void setUnitSize(Machine::MemoryUnitSize size);

        void selectAll();
        void clearData();
        void clearSelected();
        void randomSelected();
        void fillSelected();

    protected:
        void resizeEvent(QResizeEvent* event) override;
        bool event(QEvent *e) override;
    private:
        Models::MemoryModel* model;
        QTableView tableView;
        HexIntDelegate delegate;
        void updateColumnCount();
    };
}

#endif // MEMORYEDITORWIDGET_H
