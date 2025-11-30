#ifndef INSTRUCTIONEDITORWIDGET_H
#define INSTRUCTIONEDITORWIDGET_H

#include <QWidget>
#include <QTableView>
#include "Instruction/instructionmodel.h"
#include "instructioneditordelegate.h"

namespace InstructionEditor {

// forward declarations
class InstructionModel;

class InstructionEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit InstructionEditorWidget(InstructionModel *model, QWidget* parent = nullptr);

private:
    InstructionModel *model;
    QTableView tableView;
    InstructionEditorDelegate delegate;
};


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORWIDGET_H
