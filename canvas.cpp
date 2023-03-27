#include <QDebug>
#include <QMatrix4x4>
#include <vector>
#include <QtMath>
#include <QPainterPath>
#include <QPolygonF>
#include <QList>
#include "mygl.h"
#include "canvas.h"

Canvas::Canvas(QWidget *parent)
{
    Q_UNUSED(parent);
    Move_flag = 0;
    image = new QImage(1000, 1000, QImage::Format_ARGB32);
    camera = new Camera(QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 0, 0, 1), 45);
}

Canvas::~Canvas()
{
    if (nullptr != image)
    {
        delete image;
    }

    if (nullptr != camera)
    {
        delete camera;
    }
}

void Canvas::mousePressEvent(QMouseEvent *e)
{
}

void Canvas::mouseMoveEvent(QMouseEvent *e)
{
    x = e->position().x();
    y = e->position().y();
    if (Move_flag == 1)
    {
        this->update();
    }
    else
    {
        Move_flag = 1;
        x_old = x;
        y_old = y;
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *e)
{
    Move_flag = 0;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    draw();
    x_old = x;
    y_old = y;
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
    auto delta = QMatrix4x4();
    if (x > x_old)
    {
        delta.rotate(-8., 0, 1, 0);
    }
    else
    {
        delta.rotate(8., 0, 1, 0);
    }
    camera->transform(std::move(delta));
    QPainter painter(image);
    for (int i = 0; i < model->nfaces(); ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            int k = (j + 1) % 3;
            vec3 start = model->vert(i, j);
            vec3 end = model->vert(i, k);
            float z_s, z_e;
            QPointF _s = camera->shot(start, z_s);
            QPointF _e = camera->shot(end, z_e);
            int rgb = qMax(0, qMin(255, qFloor(-(z_s + z_e) * .5 * 255. / 4.)));
            painter.setPen(QColor(rgb, rgb, rgb));
            painter.drawLine(_s, _e);
        }
    }
    image->mirror();
    QPainter painter1(this);
    painter1.drawImage(0, 0, *image);
}
