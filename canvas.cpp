#include "canvas.h"
#include <QDebug>
#include <QMatrix4x4>
#include "mygl.h"
#include <vector>
#include <QtMath>
#include <QPainterPath>
#include <QPolygonF>
#include <QList>

Canvas::Canvas(QWidget *parent)
{
    Q_UNUSED(parent);
    Move_flag = 0;
    image = new QImage(1000, 1000, QImage::Format_ARGB32);
}

void Canvas::mousePressEvent(QMouseEvent *e)
{
    //qInfo("Oh");
    //draw();
}

void Canvas::mouseMoveEvent(QMouseEvent *e)
{
    //x = e->position().x();
    //y = e->position().y();
    //if (Move_flag == 1)
    //{
    //    this->update();
    //}
    //else {
    //    Move_flag = 1;
    //    x_old = x;
    //    y_old = y;
    //}
}

void Canvas::mouseReleaseEvent(QMouseEvent *e)
{
    //Q_UNUSED(e);
    //Move_flag = 0;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    //Q_UNUSED(event);

    //QPainter painter(image);
    //painter.setPen(QColor(255, 0, 0));
    //painter.drawLine(QPointF(x_old, y_old), QPointF(x, y));
    //x_old = x;
    //y_old = y;
    //QPainter painter1(this);
    //painter1.drawImage(0, 0, *image);
    draw();
}

void Canvas::draw()
{
    Model* model = ((Mygl*)parentWidget())->model();
    if (nullptr == model)
    {
        return;
    }
    quint32 *ptr;
    ptr = (quint32*)image->bits();
    for (int i = 0; i < 1000000; ++i)
    {
        ptr[i] = 0;
    }
    update();
    QPainter painter(image);
    float a = 1. / qTan(0.5 * qDegreesToRadians(45));
    auto tf = QMatrix4x4(a, 0, 0, 0, 0, a, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0);
    for (int i = 0; i < model->nfaces(); ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int rgb = 255 - qMax(0, qMin(255, (int)((model->vert(i, j).z + 1.) / 2. * 255.)));
            painter.setPen(QColor(rgb, rgb, rgb));
            painter.drawLine(vec3ToQPointF(model->vert(i, j), tf),
                             vec3ToQPointF(model->vert(i, (j + 1) % 3), tf));
        }
        //auto path = QPainterPath();
        //auto poly = QPolygonF(QList<QPointF> {
        //                        vec3ToQPointF(model->vert(i, 0), tf),
        //                        vec3ToQPointF(model->vert(i, 1), tf),
        //                        vec3ToQPointF(model->vert(i, 2), tf),
        //                      });
        //auto norm = model->normal(i, 0) + model->normal(i, 1) + model->normal(i, 2);
        //norm = norm.normalized();
        //int rgb = 255 * qAbs(norm.z);
        //painter.setBrush(QColor(rgb, rgb, rgb));
        //path.addPolygon(poly);
        //painter.drawPath(path);
    }
    image->mirror();
    QPainter painter1(this);
    painter1.drawImage(0, 0, *image);
}

static QPointF vec3ToQPointF(vec3 v, QMatrix4x4& tf)
{
    QVector3D _v = QVector3D(v.x, v.y, v.z - 3.);
    auto _r = tf.map(_v);
    return QPointF(QPointF((_r.x() + 1.) * 500, (_r.y() + 1.) * 500));
}
