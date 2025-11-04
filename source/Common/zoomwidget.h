#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>
#include <QShortcut>
#include <QKeySequence>
#include <QEvent>

class ZoomWidget : public QWidget {
    Q_OBJECT

public:
    explicit ZoomWidget(QWidget *parent = nullptr);

    double zoomFactor() const { return m_zoomFactor; }

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();
    void setZoom(double factor);

signals:
    void zoomChanged(double factor);

protected:
    virtual void onZoomChanged(double factor);

    bool eventFilter(QObject *obj, QEvent *event) override;

protected:
    double m_zoomFactor = 1.0;
};


#endif // ZOOMWIDGET_H
