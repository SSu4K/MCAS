#include "valuedisplaywidget.h"

#include <QHBoxLayout>
#include <QLabel>
#include <QFont>
#include <QPalette>

static QColor blendColors(const QColor& base, const QColor& overlay, qreal alpha)
{
    return QColor(
        base.red()   * (1.0 - alpha) + overlay.red()   * alpha,
        base.green() * (1.0 - alpha) + overlay.green() * alpha,
        base.blue()  * (1.0 - alpha) + overlay.blue()  * alpha,
        base.alpha()  * (1.0 - alpha) + overlay.alpha()  * alpha
        );
}

ValueDisplayWidget::ValueDisplayWidget(QWidget* parent)
    : QWidget(parent),
    m_label(""),
    m_bitWidth(32),
    m_base(DisplayBase::Hexadecimal),
    m_value(0)
{
    m_labelWidget = new QLabel(this);
    m_valueWidget = new QLabel(this);

    QFont monoFont;
    monoFont.setFamily("Courier");
    monoFont.setStyleHint(QFont::Monospace);
    m_valueWidget->setFont(monoFont);

    m_labelWidget->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_valueWidget->setAlignment(Qt::AlignRight | Qt::AlignVCenter);

    auto* layout = new QHBoxLayout(this);
    layout->setContentsMargins(4, 2, 4, 2);
    layout->addWidget(m_labelWidget);
    layout->addStretch();
    layout->addWidget(m_valueWidget);

    setLayout(layout);

    setActive(true);
    setDirty(false);
    setValue(0);
}

QString ValueDisplayWidget::label() const
{
    return m_label;
}

void ValueDisplayWidget::setLabel(const QString& label)
{
    m_label = label;
    m_labelWidget->setText(label);
}

int ValueDisplayWidget::bitWidth() const
{
    return m_bitWidth;
}

void ValueDisplayWidget::setBitWidth(int bits)
{
    m_bitWidth = bits;
    setValue(m_value);
}

ValueDisplayWidget::DisplayBase ValueDisplayWidget::displayBase() const
{
    return m_base;
}

void ValueDisplayWidget::setDisplayBase(DisplayBase base)
{
    m_base = base;
    setValue(m_value);
}

void ValueDisplayWidget::setValue(quint32 value)
{
    setDirty(value != m_value);
    m_value = value;

    QString text;
    switch (m_base)
    {
    case DisplayBase::Hexadecimal:
        text = "0x" + QString("%1")
                   .arg(value, m_bitWidth / 4, 16, QChar('0')).toUpper();
        break;

    case DisplayBase::UnsignedDecimal:
        text = QString::number(value);
        break;

    case DisplayBase::SignedDecimal:
    {
        int32_t signedValue = static_cast<int32_t>(
                                  value << (32 - m_bitWidth)) >> (32 - m_bitWidth);
        text = QString::number(signedValue);
        break;
    }
    }

    m_valueWidget->setText(text);
}

void ValueDisplayWidget::setDirty(bool changedThisCycle)
{
    QPalette pal = palette();

    if (changedThisCycle){
        QColor base = Qt::transparent;
        QColor highlight = pal.color(QPalette::Highlight);

        QColor blended = blendColors(base, highlight, 0.5);
        pal.setColor(QPalette::Window, blended);
    }
    else
        pal.setColor(QPalette::Window, Qt::transparent);

    setAutoFillBackground(changedThisCycle);
    setPalette(pal);
}

void ValueDisplayWidget::setActive(bool active)
{
    m_labelWidget->setEnabled(active);
    m_valueWidget->setEnabled(active);
}

QSize ValueDisplayWidget::sizeHint() const
{
    return layout()->sizeHint();
}

QSize ValueDisplayWidget::minimumSizeHint() const
{
    return layout()->minimumSize();
}

