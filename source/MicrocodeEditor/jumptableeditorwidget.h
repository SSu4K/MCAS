#ifndef JUMPTABLEEDITORWIDGET_H
#define JUMPTABLEEDITORWIDGET_H

#include "MicrocodeEditor/jumptableeditordelegate.h"
#include "Common/zoomwidget.h"

// forward declarations
class QTableView;

namespace MicrocodeEditor {
    // forward declarations
    class JumpTableModel;

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
