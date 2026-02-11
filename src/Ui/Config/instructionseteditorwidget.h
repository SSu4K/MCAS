#ifndef INSTRUCTIONSETEDITORWIDGET_H
#define INSTRUCTIONSETEDITORWIDGET_H

#include "uiLib_export.h"

#include <QTableView>
#include <Common/zoomwidget.h>

namespace Models {class InstructionSetModel;}

namespace Ui {

class UILIB_API InstructionSetEditorWidget : public ZoomWidget
{
    Q_OBJECT
    public:
        explicit InstructionSetEditorWidget(Models::InstructionSetModel* instructionSetModel, QWidget* parent = nullptr);

    protected:
        // void onZoomChanged(double factor) override;

    private:
        // void resizeColumnsToFit();
        Models::InstructionSetModel* model;
        QTableView tableView;
        // MicrocodeEditorDelegate delegate;

        const double baseFontSize = 10.0;
        // friend bool MicrocodeEditorDelegate::eventFilter(QObject* editor, QEvent* event);
};

} // namespace Ui

#endif // INSTRUCTIONSETEDITORWIDGET_H
