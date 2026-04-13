#ifndef BREAKPOINTWIDGET_H
#define BREAKPOINTWIDGET_H

#include "uiLib_export.h"

#include <QWidget>
#include <QTableView>
#include <QPushButton>

#include "breakpointdelegate.h"
#include "Instruction/breakpointmodel.h"

namespace Ui {

class UILIB_API BreakpointWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BreakpointWidget(Models::BreakpointModel* model,
                              QWidget* parent = nullptr);

private:
    Models::BreakpointModel* model;

    QTableView tableView;
    BreakpointDelegate delegate;

    QPushButton addButton;
    QPushButton removeButton;
    QPushButton clearButton;

    void setupUi();
    void setupConnections();
};

} // namespace Ui

#endif // BREAKPOINTWIDGET_H
