#include "gswidget.h"
#include "../untitled2/Predicates.h"
#include <QOpenGLFunctions>
#include <QMouseEvent>
#include <QPainterPath>

GSWidget::GSWidget(QWidget *centralwidget)
    : QOpenGLWidget(centralwidget)
{
    setMouseTracking(true);
    polygonDrawn = false;
    m_mainPix = QPixmap(2000, 2000);
    m_mainPix.fill(Qt::white);
    m_points.clear();
}

void GSWidget::mousePressEvent(QMouseEvent *event)
{
    if(!polygonDrawn)
    {
        QPainter tempPainter(&m_mainPix);
        tempPainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
        if(m_points.size() == 0)
            tempPainter.drawPoint(event->pos());
        else
            tempPainter.drawLine(m_points[m_points.size() - 1], event->pos());
        m_points.push_back(event->pos());
        m_polygon << m_points[m_points.size() - 1];
    }
    update();
}

void GSWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(polygonDrawn)
    {
        QPainter tempPainter(&m_mainPix);
        tempPainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
        QBrush m_brush;
        m_brush.setStyle(Qt::SolidPattern);
        QPainterPath m_painterpath;
        m_painterpath.addPolygon(m_polygon);
        tempPainter.drawPolygon(m_polygon);
        int intersections = 0;
        int pos1 = -2, pos2 = -2;
        auto* pa = new double[2];
        auto* pb = new double[2];
        auto* pc = new double[2];
        for(size_t i = 0; i < m_points.size() - 1; i++)
        {
            double K, B;
            int crossX = 0, crossY = 0;
            if(m_points[i+1].x() != m_points[i].x())
            {
                K = ((double)m_points[i+1].y() - m_points[i].y()) / (m_points[i+1].x() - m_points[i].x());
                B = m_points[i].y() - K * m_points[i].x();
                crossX = (event->pos().y() - B) / K;
                crossY = event->pos().y();
            }
            int minX = std::min(m_points[i+1].x(), m_points[i].x());
            int maxX = std::max(m_points[i+1].x(), m_points[i].x());
            int minY = std::min(m_points[i+1].y(), m_points[i].y());
            int maxY = std::max(m_points[i+1].y(), m_points[i].y());
            if(crossX >= minX && crossX <= maxX && crossY >= minY && crossY <= maxY && crossX >= event->pos().x() && m_points[i+1].x() != m_points[i].x())
            {
                if((crossY == m_points[i].y() && crossX == m_points[i].x()) || (crossY == m_points[i+1].y() && crossX == m_points[i+1].x()))
                {
                    pa[0] = event->pos().x();
                    pa[1] = event->pos().y();
                    pb[0] = event->pos().x() + 1;
                    pb[1] = event->pos().y();
                    if(crossY == m_points[i].y())
                    {
                        pc[0] = m_points[i+1].x();
                        pc[1] = m_points[i+1].y();
                    }
                    else if(crossY == m_points[i+1].y())
                    {
                        pc[0] = m_points[i].x();
                        pc[1] = m_points[i].y();
                    }
                    if(pos1 == -2)
                        pos1 = exact::orient2d(pa, pb, pc);
                    else
                        pos2 = exact::orient2d(pa, pb, pc);
                    if(pos1 != pos2 && (pos2 == -2 || pos1 == -2))
                        intersections++;
                    if(pos1 == pos2 && pos1 != -2 && pos1 != -2)
                        intersections++;
                    if(pos2 != -2)
                    {
                        pos1 = -2;
                        pos2 = -2;
                    }
                }
                else
                    intersections++;
            }
        }
        if(intersections % 2 == 1)
        {
            m_brush.setColor(Qt::cyan);
            tempPainter.fillPath(m_painterpath, m_brush);
        }
        else
        {
            m_brush.setColor(Qt::blue);
            tempPainter.fillPath(m_painterpath, m_brush);
        }
    }
    update();
}

void GSWidget::paintEvent(QPaintEvent */*event*/)
{
    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();
}

void GSWidget::cleanPoints()
{
    polygonDrawn = false;

    auto g = geometry();
    h = g.height();
    w = g.width();

    m_mainPix = QPixmap(w, h);
    m_mainPix.fill(Qt::white);

    m_painter.begin(this);
    m_painter.drawPixmap(0, 0, m_mainPix);
    m_painter.end();

    m_points.clear();
    m_polygon.clear();
}

void GSWidget::endPolygon()
{
    if(m_points.size() > 2 && !polygonDrawn)
    {
        QPainter tempPainter(&m_mainPix);
        tempPainter.setPen(QPen(Qt::black, 3, Qt::SolidLine, Qt::FlatCap));
        tempPainter.drawLine(m_points[m_points.size() - 1], m_points[0]);
        m_points.push_back(m_points[0]);
        m_polygon << m_points[m_points.size() - 1];
        QBrush m_brush;
        m_brush.setColor(Qt::black);
        m_brush.setStyle(Qt::SolidPattern);
        QPainterPath m_painterpath;
        m_painterpath.addPolygon(m_polygon);
        tempPainter.drawPolygon(m_polygon);
        tempPainter.fillPath(m_painterpath, m_brush);
        polygonDrawn = true;
    }
    update();
}
