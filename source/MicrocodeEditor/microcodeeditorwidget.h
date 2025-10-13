#ifndef MICROCODEEDITORWIDGET_H
#define MICROCODEEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include "microcodemodel.h"
#include "microcodeeditordelegate.h"
#include "zoomwidget.h"

namespace MicrocodeEditor{

    class MicrocodeEditorWidget : public ZoomWidget {
        Q_OBJECT
    public:
        MicrocodeModel* m_model = nullptr;

        explicit MicrocodeEditorWidget(QWidget* parent = nullptr);

        MicrocodeModel* model() const { return m_model; }
        QTableView* tableView() const { return m_tableView; }

    protected:
        void onZoomChanged(double factor) override;

    private:
        void resizeColumnsToFit();

        QTableView* m_tableView = nullptr;
        MicrocodeEditorDelegate* m_delegate = nullptr;
        const double baseFontSize = 10.0;

        friend bool MicrocodeEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };
}
#endif // MICROCODEEDITORWIDGET_H
