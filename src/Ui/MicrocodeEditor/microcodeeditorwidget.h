#ifndef MICROCODEEDITORWIDGET_H
#define MICROCODEEDITORWIDGET_H

#include <QTableView>

#include "microcodeeditordelegate.h"
#include "Common/zoomwidget.h"
#include "Microcode/microcodemodel.h"

namespace MicrocodeEditor{
    // forward declarations
    class MicrocodeModel;

    class MicrocodeEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWidget(MicrocodeModel* microcodeModel, QWidget* parent = nullptr);

    protected:
        // void onZoomChanged(double factor) override;

    private:
        // void resizeColumnsToFit();
        MicrocodeModel* model;
        QTableView tableView;
        MicrocodeEditorDelegate delegate;

        const double baseFontSize = 10.0;
        friend bool MicrocodeEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };
}
#endif // MICROCODEEDITORWIDGET_H
