#ifndef GSWIDGET_H
#define GSWIDGET_H

#include <QOpenGLWidget>
#include <QPainter>

class GSWidget : public QOpenGLWidget
{
public:
    GSWidget(QWidget *centralwidget);

    void cleanPoints();
    void endPolygon();

    bool paintStarted;
    bool polygonDrawn;

protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void paintEvent(QPaintEvent *event);

private:
    int h, w;

    QPixmap m_mainPix;
    QPainter m_painter;
    QPolygon m_polygon;
    std::vector<QPoint> m_points;
};

#endif // GSWIDGET_H
