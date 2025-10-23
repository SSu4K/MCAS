#include "zoomwidget.h"
#include <QKeySequence>
#include <QTableView>
#include <QDebug>
#include <QShortcut>

ZoomWidget::ZoomWidget(QWidget *parent)
    : QWidget(parent)
{
    // Setup standard zoom shortcuts
    new QShortcut(QKeySequence::ZoomIn,  this, SLOT(zoomIn()));
    new QShortcut(QKeySequence::ZoomOut, this, SLOT(zoomOut()));
    new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_0), this, SLOT(resetZoom()));
}

void ZoomWidget::zoomIn()  { setZoom(m_zoomFactor * 1.1); }
void ZoomWidget::zoomOut() { setZoom(m_zoomFactor / 1.1); }
void ZoomWidget::resetZoom() { setZoom(1.0); }

void ZoomWidget::setZoom(double factor)
{
    double clamped = std::clamp(factor, 0.5, 3.0);
    if (std::abs(clamped - m_zoomFactor) < 0.001)
        return;

    m_zoomFactor = clamped;
    emit zoomChanged(m_zoomFactor);
    onZoomChanged(m_zoomFactor);
}

void ZoomWidget::onZoomChanged(double factor)
{
    Q_UNUSED(factor);
}

bool ZoomWidget::eventFilter(QObject *obj, QEvent *event)
{
    return QWidget::eventFilter(obj, event);
}

