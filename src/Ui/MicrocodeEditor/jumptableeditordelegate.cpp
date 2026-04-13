#include <QKeyEvent>
#include <QLineEdit>
#include <QTableView>

#include "jumptableeditordelegate.h"
#include "Microcode/jumptablemodel.h"
#include "Microcode/jumptabledata.h"
#include "jumptableeditorwidget.h"

using namespace Models;
using namespace Ui;
using namespace Microcode;

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

    auto* view = &owner->table;
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

    return QStyledItemDelegate::eventFilter(watched, event);
}
