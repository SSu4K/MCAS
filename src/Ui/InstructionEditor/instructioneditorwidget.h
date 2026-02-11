#ifndef INSTRUCTIONEDITORWIDGET_H
#define INSTRUCTIONEDITORWIDGET_H

#include "uiLib_export.h"

#include <QWidget>
#include <QTableView>
#include "Instruction/instructionmodel.h"
#include "instructioneditordelegate.h"

// forward declarations
namespace Models {
    class InstructionModel;
}

namespace Ui {

class UILIB_API InstructionEditorWidget : public QWidget {
    Q_OBJECT
public:
    explicit InstructionEditorWidget(Models::InstructionModel *model, QWidget* parent = nullptr);

private:
    Models::InstructionModel *model;
    QTableView tableView;
    InstructionEditorDelegate delegate;
};


} // namespace InstructionEditor

#endif // INSTRUCTIONEDITORWIDGET_H
