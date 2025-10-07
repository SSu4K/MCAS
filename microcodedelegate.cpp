#include "microcodedelegate.h"
#include "microcodeeditorwidget.h"
#include "hexintdelegate.h"
#include <QLineEdit>
#include <QSpinBox>
#include <QPainter>
#include <QStyleOptionViewItem>
#include <QStyledItemDelegate>
#include <QSpinBox>
#include <QLineEdit>
#include <QComboBox>
#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>
#include <QtLogging>
#include <qtimer.h>

QWidget* MicrocodeDelegate::createEditor(QWidget* parent,
                                         const QStyleOptionViewItem& option,
                                         const QModelIndex& index) const
{
    const int col = index.column();
    QWidget* editor = nullptr;

    if (Instruction::validStringValues.contains(col)) {
        auto* enumDelegate = new EnumDelegate(const_cast<MicrocodeDelegate*>(this));
        enumDelegate->setItems(Instruction::validStringValues.value(col));
        editor = enumDelegate->createEditor(parent, option, index);
    }
    else if (col == InstructionField::constant || col == InstructionField::address) {
        auto* hexDelegate = new HexIntDelegate(parent);
        editor = hexDelegate->createEditor(parent, option, index);
    }
    else {
        editor = new QLineEdit(parent);
    }

    // Remember the editor
    m_currentEditor = editor;

    // Install event filter on editor and its line edit if applicable
    editor->installEventFilter(const_cast<MicrocodeDelegate*>(this));

    if (auto* combo = qobject_cast<QComboBox*>(editor)) {
        if (auto* line = combo->lineEdit()) {
            line->installEventFilter(const_cast<MicrocodeDelegate*>(this));
            QTimer::singleShot(0, line, [line]{ line->selectAll(); });
        }
    } else if (auto* line = qobject_cast<QLineEdit*>(editor)) {
        QTimer::singleShot(0, line, [line]{ line->selectAll(); });
    }

    return editor;
}

bool MicrocodeDelegate::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() != QEvent::KeyPress)
        return QStyledItemDelegate::eventFilter(watched, event);

    auto* keyEvent = static_cast<QKeyEvent*>(event);
    auto* owner = qobject_cast<MicrocodeEditorWidget*>(parent());
    if (!owner) return QStyledItemDelegate::eventFilter(watched, event);

    auto* view = owner->m_tableView;
    auto* model = view ? view->model() : nullptr;
    if (!model) return QStyledItemDelegate::eventFilter(watched, event);

    QModelIndex current = view->currentIndex();
    if (!current.isValid()) return false;

    QWidget* editor = m_currentEditor;
    if (!editor) return false;

    // Let Escape cancel edit normally
    if (keyEvent->key() == Qt::Key_Escape)
        return QStyledItemDelegate::eventFilter(watched, event);

    auto commitCloseAndEdit = [&](const QModelIndex& next) {
        emit commitData(editor);
        emit closeEditor(editor, QAbstractItemDelegate::NoHint);
        if (next.isValid()) {
            view->setCurrentIndex(next);
            view->edit(next);
        }
    };

    const bool shift = keyEvent->modifiers() & Qt::ShiftModifier;

    switch (keyEvent->key()) {
    case Qt::Key_Return:
    case Qt::Key_Enter:
        if (shift) {
            if (auto* microModel = qobject_cast<MicrocodeModel*>(model)) {
                microModel->insertInstruction(current.row() + 1);
                commitCloseAndEdit(model->index(current.row() + 1, current.column()));
            }
        } else {
            commitCloseAndEdit(model->index(current.row() + 1, current.column()));
        }
        return true;

    case Qt::Key_Tab:
    case Qt::Key_Right:
        commitCloseAndEdit(model->index(current.row(), current.column() + 1));
        return true;

    case Qt::Key_Left:
        commitCloseAndEdit(model->index(current.row(), current.column() - 1));
        return true;

    case Qt::Key_Down:
        commitCloseAndEdit(model->index(current.row() + 1, current.column()));
        return true;

    case Qt::Key_Up:
        commitCloseAndEdit(model->index(current.row() - 1, current.column()));
        return true;

    default:
        break;
    }

    return QStyledItemDelegate::eventFilter(watched, event);
}


void MicrocodeDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const {
    const QVariant value = index.data(Qt::EditRole);
    const int col = index.column();

    if (Instruction::validStringValues.contains(col)) {
        // Use EnumDelegate for dropdown string fields
        EnumDelegate enumDel(const_cast<MicrocodeDelegate*>(this));
        enumDel.setItems(Instruction::validStringValues[col]);
        enumDel.setEditorData(editor, index);
    }
    else if (col == InstructionField::address || col == InstructionField::constant) {
        // Use HexIntDelegate for integer/hex fields
        HexIntDelegate hexDel(const_cast<MicrocodeDelegate*>(this));
        hexDel.setEditorData(editor, index);
    }
    else if (auto* line = qobject_cast<QLineEdit*>(editor)) {
        line->setText(value.toString());
        line->selectAll();
    }
}

void MicrocodeDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                     const QModelIndex& index) const {
    const int col = index.column();

    if (Instruction::validStringValues.contains(col)) {
        EnumDelegate enumDel(const_cast<MicrocodeDelegate*>(this));
        enumDel.setItems(Instruction::validStringValues[col]);
        enumDel.setModelData(editor, model, index);
    }
    else if (col == InstructionField::address || col == InstructionField::constant) {
        HexIntDelegate hexDel(const_cast<MicrocodeDelegate*>(this));
        hexDel.setModelData(editor, model, index);
    }
    else if (auto* line = qobject_cast<QLineEdit*>(editor)) {
        model->setData(index, line->text(), Qt::EditRole);
    }

    m_currentEditor = nullptr;
}

void MicrocodeDelegate::updateEditorGeometry(QWidget* editor,
                          const QStyleOptionViewItem& option,
                          const QModelIndex&) const {
    editor->setGeometry(option.rect);
}

