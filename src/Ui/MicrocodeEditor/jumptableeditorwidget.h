#ifndef JUMPTABLEEDITORWIDGET_H
#define JUMPTABLEEDITORWIDGET_H

#include <QTableView>

#include "MicrocodeEditor/jumptableeditordelegate.h"
#include "Common/zoomwidget.h"
#include "Microcode/jumptablemodel.h"

// forward declarations
class QTableView;

namespace MicrocodeEditor {
    // forward declarations
    class JumpTableModel;

    class JumpTableEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        explicit JumpTableEditorWidget(JumpTableModel* jumpTableModel, QWidget* parent = nullptr);

        void resizeColumnsToFit();

    private:
        QTableView table;
        JumpTableEditorDelegate delegate;
        JumpTableModel* model;

        friend bool JumpTableEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };

}
#endif // JUMPTABLEEDITORWIDGET_H
