#ifndef INSTRUCTIONEDITORWIDGET_H
#define INSTRUCTIONEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include <QVBoxLayout>
#include <QPushButton>
#include <QHeaderView>

#include "instructioneditormodel.h"
#include "instructioneditordelegate.h"

namespace InstructionEditor {

class InstructionEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit InstructionEditorWidget(QWidget* parent = nullptr);

private:
    QTableView* tableView;
    InstructionEditorModel* model;
    InstructionEditorDelegate* delegate;
};


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORWIDGET_H
