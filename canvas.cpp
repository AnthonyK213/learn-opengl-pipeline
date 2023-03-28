#define DELETE(x) if(nullptr!=x)delete x;

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
    setParent(parent);
    _move_flag = 0;
    image = new QImage(1000, 1000, QImage::Format_ARGB32);
    camera = new Camera(QMatrix4x4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 3, 0, 0, 0, 1), 45);
    show();
}

Canvas::~Canvas()
{
    DELETE(image);
    DELETE(camera);
}

void Canvas::mouseMoveEvent(QMouseEvent *e)
{
    x = e->position().x();
    y = e->position().y();
    if (e->buttons() == Qt::MouseButton::RightButton)
    {
        if (_move_flag == 1)
        {
            this->update();
        }
        else
        {
            _move_flag = 1;
            x_old = x;
            y_old = y;
        }
    }
}

void Canvas::mouseReleaseEvent(QMouseEvent *e)
{
    _move_flag = 0;
}

void Canvas::paintEvent(QPaintEvent *event)
{
    draw();
    x_old = x;
    y_old = y;
}

void Canvas::draw()
{
    Model* model = ((Mygl*)(parentWidget()->parentWidget()))->model();
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
    if (_move_flag)
    {
        auto delta = QMatrix4x4();
        auto _data = camera->tf().column(0);
        delta.rotate((float)(x_old - x) * .4, 0, 1, 0);
        delta.rotate((float)(y_old - y) * .4, _data[0], _data[1], _data[2]);
        camera->transform(std::move(delta));
    }
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
            int alpha = 255 - qMax(0, qMin(255, qFloor(-(z_s + z_e) * .5 * 255. / 4.)));
            painter.setPen(QColor(0, 0, 0, alpha));
            painter.drawLine(_s, _e);
        }
    }
    image->mirror();
    QPainter painter1(this);
    painter1.drawImage(0, 0, *image);
}

void Canvas::changeFovy(int fovy)
{
    this->camera->setFovy(fovy);
    update();
}
