#ifndef CHARTSCROLLBAR_H
#define CHARTSCROLLBAR_H

#include <QtCharts>
#include <QFile>
#include <QTextStream>
#include <QDateTime>

#include <QtWidgets>

class ZoomableChartView : public QChartView {
public:
    ZoomableChartView(QChart *chart, QWidget *parent = nullptr)
        : QChartView(chart, parent), isPanning(false), currentZoom(1.0) {
        setRenderHint(QPainter::Antialiasing);
        setRubberBand(QChartView::RectangleRubberBand);
    }

protected:
    void wheelEvent(QWheelEvent *event) override {
        if (event->angleDelta().y() > 0) {
            chart()->zoomIn();
            currentZoom *= 1.2;
        } else {
            chart()->zoomOut();
            currentZoom *= 0.8;
        }
        emit zoomChanged(currentZoom);
        event->accept();
    }

    void mousePressEvent(QMouseEvent *event) override {
        if (event->button() == Qt::MiddleButton) {
            isPanning = true;
            lastMousePos = event->pos();
            setCursor(Qt::ClosedHandCursor);
            event->accept();
        } else {
            QChartView::mousePressEvent(event);
        }
    }

    void mouseMoveEvent(QMouseEvent *event) override {
        if (isPanning) {
            QPoint delta = event->pos() - lastMousePos;
            chart()->scroll(-delta.x(), delta.y());
            lastMousePos = event->pos();
            event->accept();
        } else {
            QChartView::mouseMoveEvent(event);
        }
    }

    void mouseReleaseEvent(QMouseEvent *event) override {
        if (event->button() == Qt::MiddleButton) {
            isPanning = false;
            setCursor(Qt::ArrowCursor);
            event->accept();
        } else {
            QChartView::mouseReleaseEvent(event);
        }
    }

private:
    bool isPanning;
    QPoint lastMousePos;
    double currentZoom;
};

#endif // CHARTSCROLLBAR_H
