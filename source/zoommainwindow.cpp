#include "zoommainwindow.h"
#include <QKeySequence>
#include <QTableView>
#include <QDebug>
#include <QShortcut>

ZoomMainWindow::ZoomMainWindow(QWidget *parent)
    : QMainWindow(parent), m_zoomFactor(1.0)
{
    // Standard shortcuts for zooming
    auto *zoomInShortcut  = new QShortcut(QKeySequence::ZoomIn,  this);
    auto *zoomOutShortcut = new QShortcut(QKeySequence::ZoomOut, this);
    auto *resetShortcut   = new QShortcut(QKeySequence(Qt::CTRL | Qt::Key_0), this);

    connect(zoomInShortcut,  &QShortcut::activated, this, &ZoomMainWindow::zoomIn);
    connect(zoomOutShortcut, &QShortcut::activated, this, &ZoomMainWindow::zoomOut);
    connect(resetShortcut,   &QShortcut::activated, this, &ZoomMainWindow::resetZoom);
}

void ZoomMainWindow::registerZoomableWidget(QWidget *widget)
{
    if (!m_zoomWidgets.contains(widget))
        m_zoomWidgets.append(widget);
}

void ZoomMainWindow::zoomIn()
{
    setZoom(m_zoomFactor * 1.1);
}

void ZoomMainWindow::zoomOut()
{
    setZoom(m_zoomFactor / 1.1);
}

void ZoomMainWindow::resetZoom()
{
    setZoom(1.0);
}

void ZoomMainWindow::setZoom(double factor)
{
    m_zoomFactor = std::clamp(factor, 0.5, 3.0);

    for (QWidget *w : m_zoomWidgets) {
        QFont font = w->font();
        font.setPointSizeF(m_baseFontSize * m_zoomFactor);
        w->setFont(font);

        // // If it’s a QTableView, adjust section sizes
        // if (auto *table = qobject_cast<QTableView*>(w)) {
        //     table->horizontalHeader()->setDefaultSectionSize(int(m_baseColumnWidth * m_zoomFactor));
        //     table->verticalHeader()->setDefaultSectionSize(int(m_baseRowHeight * m_zoomFactor));
        // }

        w->update();
    }
}
