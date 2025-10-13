#ifndef ZOOMMAINWINDOW_H
#define ZOOMMAINWINDOW_H

#include <QMainWindow>

class ZoomMainWindow : public QMainWindow
{
    Q_OBJECT
public:
    explicit ZoomMainWindow(QWidget *parent = nullptr);
    void registerZoomableWidget(QWidget *widget);
    double zoomFactor() const { return m_zoomFactor; }

public slots:
    void zoomIn();
    void zoomOut();
    void resetZoom();

protected:
    void setZoom(double factor);

private:
    QList<QWidget*> m_zoomWidgets;
    double m_zoomFactor;
    const double m_baseFontSize = 10.0;
    const double m_baseColumnWidth = 100.0;
    const double m_baseRowHeight = 25.0;
};

#endif // ZOOMMAINWINDOW_H
