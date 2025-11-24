#ifndef INSTRUCTIONEDITORWIDGET_H
#define INSTRUCTIONEDITORWIDGET_H

#include <QWidget>
#include <QTableView>

namespace InstructionEditor {

// forward declarations
class InstructionEditorModel;
class InstructionEditorDelegate;

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
