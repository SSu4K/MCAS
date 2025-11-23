#ifndef MICROCODEEDITORWIDGET_H
#define MICROCODEEDITORWIDGET_H

#include <QTableView>

#include "microcodeeditordelegate.h"
#include "Common/zoomwidget.h"

namespace MicrocodeEditor{
    // forward declarations
    class MicrocodeModel;

    class MicrocodeEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        explicit MicrocodeEditorWidget(QWidget* parent = nullptr);

        MicrocodeModel* model() const { return m_model; }
        QTableView* tableView() const { return m_tableView; }

    protected:
        // void onZoomChanged(double factor) override;

    private:
        // void resizeColumnsToFit();
        MicrocodeModel* m_model = nullptr;
        QTableView* m_tableView = nullptr;
        MicrocodeEditorDelegate* m_delegate = nullptr;
        const double baseFontSize = 10.0;

        friend bool MicrocodeEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };
}
#endif // MICROCODEEDITORWIDGET_H
