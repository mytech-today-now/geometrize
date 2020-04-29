#include "imagetaskgraphicsview.h"

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QKeyEvent>
#include <QString>
#include <QTabletEvent>
#include <QWheelEvent>

namespace
{

/*
void printTabletEventInfo(const QTabletEvent* const event)
{
    if(!event) {
        return;
    }
    const QString action = [event]() {
        if(event->type() == QEvent::TabletMove)
            return "moved";
        else if(event->type() == QEvent::TabletPress)
            return "pressed";
        else if(event->type() == QEvent::TabletRelease)
            return "lifted";
        return "unknown";
    }();

    qInfo() << action << " at " << QString::number(event->posF().x(), 'g', 8) << ", " << QString::number(event->posF().y(), 'g', 8)
            << " pressure = " << QString::number(event->pressure() * 100.0, 'g', 8)
            << " tangentialPressure = " << QString::number(event->tangentialPressure(), 'g', 8)
            << " rotation = " << QString::number(event->rotation(), 'g', 8)
            << " xTilt = " << QString::number(event->xTilt(), 'g', 8)
            << " yTilt = " << QString::number(event->yTilt(), 'g', 8);
}
*/

}

namespace geometrize
{

namespace scene
{

ImageTaskGraphicsView::ImageTaskGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    setInteractive(true);
    setMouseTracking(true);
    setTransformationAnchor(QGraphicsView::AnchorUnderMouse);
    setDragMode(QGraphicsView::ScrollHandDrag);
    setViewportUpdateMode(QGraphicsView::FullViewportUpdate);

    populateUi();
}

bool ImageTaskGraphicsView::viewportEvent(QEvent* event)
{
    //printTabletEventInfo(dynamic_cast<QTabletEvent*>(event));

    if(event->type() == QEvent::TabletMove) {
        QApplication::sendEvent(scene(), event);
        event->accept();
    } else if(event->type() == QEvent::TabletPress) {
        QApplication::sendEvent(scene(), event);
        event->accept();
    } else if(event->type() == QEvent::TabletRelease) {
        QApplication::sendEvent(scene(), event);
        event->accept();
    } else {
        return QGraphicsView::viewportEvent(event);
    }
    return true;
}

void ImageTaskGraphicsView::wheelEvent(QWheelEvent* e)
{
    if(e->modifiers() & Qt::ControlModifier) {
        // Pass down to scene/children in scene if control is pressed
        QGraphicsView::wheelEvent(e);
        return;
    }

    // Handle zooming
    if (e->angleDelta().x() == 0) {
        const QPoint pos{e->pos()};
        const QPointF posf{mapToScene(pos)};

        const double angle{static_cast<double>(e->angleDelta().y())};
        double by{0.0};
        if(angle > 0) {
            by = 1 + (angle / 360 * 0.1);
        } else if (angle < 0) {
            by = 1 - (-angle / 360 * 0.1);
        } else {
            by = 1;
        }

        scale(by, by);

        const int w{viewport()->width()};
        const int h{viewport()->height()};
        const double wf{mapToScene(QPoint(w - 1, 0)).x() -mapToScene(QPoint(0,0)).x()};
        const double hf{mapToScene(QPoint(0, h - 1)).y() - mapToScene(QPoint(0,0)).y()};
        const double lf{posf.x() - pos.x() * wf / w};
        const double tf{posf.y() - pos.y() * hf / h};

        ensureVisible(lf, tf, wf, hf, 0, 0);
        const QPointF newPos{mapToScene(pos)};
        ensureVisible(QRectF(QPointF(lf, tf) - newPos + posf, QSizeF(wf, hf)), 0, 0);

        e->accept();
    }
}

void ImageTaskGraphicsView::mousePressEvent(QMouseEvent* event)
{
    QGraphicsView::mousePressEvent(event);
}

void ImageTaskGraphicsView::mouseReleaseEvent(QMouseEvent* event)
{
    QGraphicsView::mouseReleaseEvent(event);
}

void ImageTaskGraphicsView::mouseMoveEvent(QMouseEvent* event)
{
    QGraphicsView::mouseMoveEvent(event);
}

void ImageTaskGraphicsView::keyPressEvent(QKeyEvent* event)
{
    QGraphicsView::keyPressEvent(event);
}

void ImageTaskGraphicsView::keyReleaseEvent(QKeyEvent* event)
{
    QGraphicsView::keyReleaseEvent(event);
}

void ImageTaskGraphicsView::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::LanguageChange) {
        populateUi();
    }
    QGraphicsView::changeEvent(event);
}

void ImageTaskGraphicsView::populateUi()
{

}

}

}
