#include "jumptableeditordelegate.h"
#include "jumptablemodel.h"
#include "jumptableeditorwidget.h"
#include <QKeyEvent>
#include <qlineedit.h>

using namespace MicrocodeEditor;

QWidget* JumpTableEditorDelegate::createEditor(QWidget* parent,
                                               const QStyleOptionViewItem& option,
                                               const QModelIndex& index) const
{
    QWidget* editor = new QLineEdit(parent);
    m_currentEditor = editor;

    // Install event filter on editor and its line edit if applicable
    editor->installEventFilter(const_cast<JumpTableEditorDelegate*>(this));
    return editor;
}

bool JumpTableEditorDelegate::eventFilter(QObject* watched, QEvent* event)
{
    if (event->type() != QEvent::KeyPress)
        return QStyledItemDelegate::eventFilter(watched, event);

    auto* keyEvent = static_cast<QKeyEvent*>(event);
    auto* owner = qobject_cast<JumpTableEditorWidget*>(parent());
    if (!owner) return QStyledItemDelegate::eventFilter(watched, event);

    auto* view = owner->m_table;
    auto* model = view ? view->model() : nullptr;
    if (!model) return QStyledItemDelegate::eventFilter(watched, event);

    QModelIndex current = view->currentIndex();
    if (!current.isValid()) return false;

    QWidget* editor = m_currentEditor;
    if (!editor) return false;

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
            if (auto* jumpTableModel = qobject_cast<JumpTableModel*>(model)) {
                jumpTableModel->insertEntry(current.row() + 1, JumpTableEntry());
                commitCloseAndEdit(model->index(current.row() + 1, current.column()));
            }
        } else {
            commitCloseAndEdit(model->index(current.row() + 1, current.column()));
        }
        return true;
    }

    return QStyledItemDelegate::eventFilter(watched, event);
}
