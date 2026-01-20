#ifndef VALUEDISPLAYWIDGET_H
#define VALUEDISPLAYWIDGET_H

#include <QWidget>
#include <QString>
#include <QtUiPlugin/QDesignerExportWidget>

class QLabel;

class ValueDisplayWidget : public QWidget
{
    Q_OBJECT

    Q_PROPERTY(QString label READ label WRITE setLabel)
    Q_PROPERTY(int bitWidth READ bitWidth WRITE setBitWidth)
    Q_PROPERTY(DisplayBase displayBase READ displayBase WRITE setDisplayBase)

public:
    enum DisplayBase
    {
        Hexadecimal,
        UnsignedDecimal,
        SignedDecimal
    };
    Q_ENUM(DisplayBase)

    explicit ValueDisplayWidget(QWidget* parent = nullptr);

    QString label() const;
    void setLabel(const QString& label);

    int bitWidth() const;
    void setBitWidth(int bits);

    DisplayBase displayBase() const;
    void setDisplayBase(DisplayBase base);

    void setValue(quint32 value);
    void setDirty(bool changedThisCycle);
    void setActive(bool active);

    QSize sizeHint() const override;
    QSize minimumSizeHint() const override;

private:
    QString m_label;
    int m_bitWidth;
    DisplayBase m_base;

#ifndef QDESIGNER
    quint32 m_value;
#endif

    QLabel* m_labelWidget;
    QLabel* m_valueWidget;
};

#endif // VALUEDISPLAYWIDGET_H
