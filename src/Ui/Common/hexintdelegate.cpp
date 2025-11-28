#include <QLineEdit>
#include <QPainter>
#include <QStyleOptionViewItem>

#include "hexintdelegate.h"
#include "Common/hexint.h"

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
        quint32 v = data.toUInt();
        qDebug() << "Editor data as int:" << v;
        QString s = HexInt::intToString(v, false, m_precision);

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
    quint32 value = text.toUInt(&ok, 16);
    qDebug() << "Input line" << text;
    qDebug() << "Parsed line as hex:" << value;
    if(!ok){
        value = static_cast<quint32>(text.toInt(&ok, 10));
        qDebug() << "Parsed line as dec:" << value;
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
