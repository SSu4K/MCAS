#ifndef MICROCODEEDITORWIDGET_H
#define MICROCODEEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include "microcodemodel.h"
#include "microcodeeditordelegate.h"

namespace MicrocodeEditor{

    class MicrocodeEditorWidget : public QWidget {
        Q_OBJECT
    public:
        MicrocodeModel* m_model = nullptr;

        explicit MicrocodeEditorWidget(QWidget* parent = nullptr);

        MicrocodeModel* model() const { return m_model; }
        QTableView* tableView() const { return m_tableView; }

    private:
        void resizeColumnsToFit();

        QTableView* m_tableView = nullptr;
        MicrocodeEditorDelegate* m_delegate = nullptr;

        friend bool MicrocodeEditorDelegate::eventFilter(QObject* editor, QEvent* event);
    };
}
#endif // MICROCODEEDITORWIDGET_H
