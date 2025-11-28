#ifndef INSTRUCTIONEDITORWIDGET_H
#define INSTRUCTIONEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include "Instruction/instructioneditormodel.h"

namespace InstructionEditor {

// forward declarations
class InstructionEditorModel;
class InstructionEditorDelegate;

class InstructionEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit InstructionEditorWidget(QWidget* parent = nullptr);
    InstructionEditorModel* getModel() {return &model;}

private:
    QTableView* tableView;
    InstructionEditorModel model;
    InstructionEditorDelegate* delegate;
};


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORWIDGET_H
