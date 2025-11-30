#ifndef JUMPTABLEEDITORWIDGET_H
#define JUMPTABLEEDITORWIDGET_H

#include <QTableView>

#include "MicrocodeEditor/jumptableeditordelegate.h"
#include "Common/zoomwidget.h"
#include "Microcode/jumptablemodel.h"

// forward declarations
namespace Models {class JumpTableModel;}

namespace Ui {
    // forward declarations

    class JumpTableEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        explicit JumpTableEditorWidget(Models::JumpTableModel* jumpTableModel, QWidget* parent = nullptr);

        void resizeColumnsToFit();

    private:
        QTableView table;
        JumpTableEditorDelegate delegate;
        Models::JumpTableModel* model;

        friend bool JumpTableEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };

}
#endif // JUMPTABLEEDITORWIDGET_H
