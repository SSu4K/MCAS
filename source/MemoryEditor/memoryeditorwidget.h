#ifndef MEMORYEDITORWIDGET_H
#define MEMORYEDITORWIDGET_H

#include <QTableView>
#include <QResizeEvent>
#include <QHeaderView>
#include <QBoxLayout>
#include <QTimer>

#include "memorymodel.h"
#include "Common/hexintdelegate.h"

namespace MemoryEditor {

    class MemoryEditorWidget : public QWidget {
        Q_OBJECT
    public:
        explicit MemoryEditorWidget(QWidget* parent = nullptr);
        void setMemory(const QByteArray& memory);

        MemoryUnitSize getUnitSize();
        void setUnitSize(MemoryUnitSize size);

    protected:
        void resizeEvent(QResizeEvent* event) override;

    private:
        QTableView* m_tableView;
        MemoryModel* m_model;
        HexIntDelegate* m_delegate;
        void updateColumnCount();
    };
}

#endif // MEMORYEDITORWIDGET_H
