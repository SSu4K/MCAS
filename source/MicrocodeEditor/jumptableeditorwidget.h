#ifndef JUMPTABLEEDITORWIDGET_H
#define JUMPTABLEEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include "MicrocodeEditor/jumptableeditordelegate.h"
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
        JumpTableEditorDelegate* m_delegate = nullptr;
        JumpTableModel* m_model;

        friend bool JumpTableEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };

}
#endif // JUMPTABLEEDITORWIDGET_H
