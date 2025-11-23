#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "hexintdelegate.h"
#include "hexint.h"

HexIntDelegate::HexIntDelegate(QObject* parent)
    : QStyledItemDelegate(parent) {}

QWidget* HexIntDelegate::createEditor(QWidget* parent,
                                      const QStyleOptionViewItem&,
                                      const QModelIndex&) const
{
    auto* line = new QLineEdit(parent);
    QFont mono("Monospace");
    mono.setStyleHint(QFont::TypeWriter);
    line->setFont(mono);
    return line;
}

void HexIntDelegate::setEditorData(QWidget* editor, const QModelIndex& index) const
{
    auto* line = qobject_cast<QLineEdit*>(editor);
    if (!line) return;

    OutputMode mode = OutputMode::Auto;
    QVariant data = index.data(Qt::EditRole);

    qDebug() << "typeID:" << data.typeId() << "name:" << data.typeName();

    if (data.typeId() == QMetaType::UInt) {
        quint32 v = data.toInt();
        QString s = HexInt::intToString(v, false, m_precision);
        qDebug() << QString::number(v, 16) << m_precision << s;
        line->setText(s);
        mode = OutputMode::Integer;
        qDebug("setEditorData type: quint32");
    }
    else {
        line->setText(data.toString());
        mode = OutputMode::String;
        qDebug("setEditorData type: QString");
    }

    line->setProperty("outputMode", static_cast<int>(mode));
    line->selectAll();
}


void HexIntDelegate::setModelData(QWidget* editor, QAbstractItemModel* model,
                                  const QModelIndex& index) const
{
    auto* line = qobject_cast<QLineEdit*>(editor);
    if (!line)
        return;

    QString text = line->text().trimmed();

    if(text.isEmpty()){
        model->setData(index, "", Qt::EditRole);
        return;
    }

    auto mode = static_cast<OutputMode>(line->property("outputMode").toInt());

    bool ok = false;
    int value = text.toInt(&ok, 10);
    if(!ok || value >= 0){
        value = text.toInt(&ok, 16);
    }

    if(!ok){
        return;
    }

    if (mode == OutputMode::Integer) {
        model->setData(index, value, Qt::EditRole);
    } else {
        model->setData(index, HexInt::intToString(value, false, m_precision), Qt::EditRole);
    }
}


void HexIntDelegate::updateEditorGeometry(QWidget* editor,
                                          const QStyleOptionViewItem& option,
                                          const QModelIndex&) const
{
    editor->setGeometry(option.rect);
}

void HexIntDelegate::paint(QPainter* painter,
                           const QStyleOptionViewItem& option,
                           const QModelIndex& index) const
{
    QStyleOptionViewItem opt(option);
    initStyleOption(&opt, index);

    const QString value = index.data(Qt::DisplayRole).toString();
    opt.text = value;
    QStyledItemDelegate::paint(painter, opt, index);
}

void HexIntDelegate::setPrecision(qsizetype precision){
    m_precision = precision;
}

qsizetype HexIntDelegate::getPrecision() const{
    return m_precision;
}
