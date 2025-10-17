#ifndef JUMPTABLEEDITORWIDGET_H
#define JUMPTABLEEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include "zoomwidget.h"
#include "jumptablemodel.h"

namespace MicrocodeEditor {

    class JumpTableEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        explicit JumpTableEditorWidget(QWidget* parent = nullptr);

        JumpTableModel* model() const { return m_model; }
        void resizeColumnsToFit();

    private:
        QTableView* m_table;
        JumpTableModel* m_model;
    };

}
#endif // JUMPTABLEEDITORWIDGET_H
