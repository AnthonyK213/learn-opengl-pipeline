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
    z_buffer = new float[1000 * 1000];
    show();
}

Canvas::~Canvas()
{
    DELETE(image);
    DELETE(camera);
    DELETE(z_buffer);
}

void Canvas::wheelEvent(QWheelEvent *e)
{
    QPoint delta = e->angleDelta();
    addFovy(-delta.y() * .03);
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
    image->fill(Qt::black);
    //quint32 *ptr = (quint32*)image->bits();
    //for (int i = 0; i < 1000000; ++i) ptr[i] = 0;
    Model* model = ((Mygl*)(parentWidget()->parentWidget()))->model();
    if (nullptr == model)
    {
        QPainter painter1(this);
        painter1.drawImage(0, 0, *image);
        return;
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
    vec3 light = (vec3 { -1, -1, -1 }).normalized();
    for (int i = 0; i < 1000 * 1000; ++i)
    {
        z_buffer[i] = -std::numeric_limits<float>::max();
    }
    for (int i = 0; i < model->nfaces(); ++i)
    {
        // Shading.
        float z0, z1, z2;
        vec3 m0 = model->vert(i, 0);
        vec3 m1 = model->vert(i, 1);
        vec3 m2 = model->vert(i, 2);
        QPointF p0 = camera->shot(m0, z0);
        QPointF p1 = camera->shot(m1, z1);
        QPointF p2 = camera->shot(m2, z2);
        QPointF pts[] = { p0, p1, p2 };
        vec3 n0 = model->normal(i, 0);
        vec3 n1 = model->normal(i, 1);
        vec3 n2 = model->normal(i, 2);
        if (this->_shade == 0)
        {
            vec3 norm = (n0 + n1 + n2).normalized();
            int rgb = (1 - norm * light) * .5 * 200;
            painter.setPen(QColor(rgb, rgb, rgb));
        }
        // Diffuse
        const TGAImage& diffuse = model->diffuse();
        // Specular
        const TGAImage& specular = model->specular();
        // UV, tangent
        vec2 uv0, uv1, uv2;
        vec3 T_;
        if (diffuse.width() > 0)
        {
            uv0 = model->uv(i, 0);
            uv1 = model->uv(i, 1);
            uv2 = model->uv(i, 2);
            vec2 e1 = uv1 - uv0;
            vec2 e2 = uv2 - uv0;
            float _det = e1.x * e2.y - e1.y * e2.x;
            float t00 = e2.y / _det;
            float t01 = -e1.y / _det;
            //float t10 = -e2.x / _det;
            //float t11 = e1.x / _det;
            vec3 E1 = m1 - m0;
            vec3 E2 = m2 - m0;
            T_.x = t00 * E1.x + t01 * E2.x;
            T_.y = t00 * E1.y + t01 * E2.y;
            T_.z = t00 * E1.z + t01 * E2.z;
        }
        // Bounding box.
        float box_xmin(std::numeric_limits<float>::max());
        float box_ymin(std::numeric_limits<float>::max());
        float box_xmax(-std::numeric_limits<float>::max());
        float box_ymax(-std::numeric_limits<float>::max());
        for (int j = 0; j < 3; ++j)
        {
            box_xmin = qMin(box_xmin, pts[j].x());
            box_ymin = qMin(box_ymin, pts[j].y());
            box_xmax = qMax(box_xmax, pts[j].x());
            box_ymax = qMax(box_ymax, pts[j].y());
        }
        // Barycentric.
        float m00 = p0.x() - p2.x();
        float m01 = p1.x() - p2.x();
        float m10 = p0.y() - p2.y();
        float m11 = p1.y() - p2.y();
        float det = (m00 * m11 - m01 * m10);
        if (qAbs(det) < 1e-5) continue;
        float invdet = 1. / det;
        float n00 = invdet * m11;
        float n01 = invdet * -m01;
        float n10 = invdet * -m10;
        float n11 = invdet * m00;
        // Raster
        int bxmin = qMax(qFloor(box_xmin), 0);
        int bymin = qMax(qFloor(box_ymin), 0);
        int bxmax = qMin(qCeil(box_xmax), 1000);
        int bymax = qMin(qCeil(box_ymax), 1000);
        for (int yPos = bymin; yPos < bymax; yPos++)
        {
            for (int xPos = bxmin; xPos < bxmax; xPos++)
            {
                float u = n00 * (xPos - p2.x()) + n01 * (yPos - p2.y());
                float v = n10 * (xPos - p2.x()) + n11 * (yPos - p2.y());
                float w = 1. - u - v;
                if (u < 0 || v < 0 || w < 0) continue;
                float _u = u / z0;
                float _v = v / z1;
                float _w = w / z2;
                float _m = _u + _v + _w;
                float pz = (_u * z0 + _v * z1 + _w * z2) / _m;
                // Smooth shading.
                int index = xPos + yPos * 1000;
                if (z_buffer[index] < pz) {
                    z_buffer[index] = pz;
                    if (this->_shade == 1)
                    {
                        auto cam_z = camera->tf().column(2);
                        vec3 n = (n0 * _u + n1 * _v + n2 * _w).normalized();
                        if (_m < 0) n = n * (-1.);
                        if (diffuse.width() > 0)
                        {
                            vec2 uv = (uv0 * _u + uv1 * _v + uv2 * _w) / _m;
                            vec3 n0 = model->normal(uv).normalized();
                            vec3 t_ = (T_ - (T_ * n) * n).normalized();
                            vec3 b_ = cross(n, t_);
                            vec3 n_ = {
                                t_.x * n0.x + b_.x * n0.y + n.x * n0.z,
                                t_.y * n0.x + b_.y * n0.y + n.y * n0.z,
                                t_.z * n0.x + b_.z * n0.y + n.z * n0.z,
                            };
                            n_ = n_.normalized();
                            vec3 r = -2. * n_ * (n_ * light) + light;
                            float diff = (1. - n_ * light) * .5;
                            float spec = pow(qMax(r.z, 0.0f), specular.get(uv.x * specular.width(), uv.y * specular.height()).bgra[0]);
                            //float spec = (vec3 { cam_z.x(), cam_z.y(), cam_z.z() } * r + 1.) * .5;
                            float indensity = diff + .6 * spec;
                            TGAColor pixel = diffuse.get(uv.x * diffuse.width(), uv.y * diffuse.height());
                            painter.setPen(QColor(qMin(255., pixel.bgra[2] * indensity),
                                                  qMin(255., pixel.bgra[1] * indensity),
                                                  qMin(255., pixel.bgra[0] * indensity)));
                        }
                        else
                        {
                            vec3 r = -2. * n * (n * light) + light;
                            float diff = (1. - n * light) * .5;
                            if ((vec3 { cam_z.x(), cam_z.y(), cam_z.z() } * r + 1.) * .5 > 0.996)
                            {
                                diff = 1.1f;
                            }
                            int rgb = 200 * diff;
                            painter.setPen(QColor(rgb, rgb, rgb));
                        }
                    }
                    painter.drawPoint(xPos, yPos);
                }
            }
        }

        //// Drawing wire frame.
        //for (int j = 0; j < 3; ++j)
        //{
        //    int k = (j + 1) % 3;
        //    vec3 start = model->vert(i, j);
        //    vec3 end = model->vert(i, k);
        //    float z_s, z_e;
        //    QPointF _s = camera->shot(start, z_s);
        //    QPointF _e = camera->shot(end, z_e);
        //    //int alpha = 255 - qMax(0, qMin(255, qFloor(-(z_s + z_e) * .5 * 255. / 4.)));
        //    //painter.setPen(QColor(0, 0, 0, alpha));
        //    painter.setPen(QColor(0, 0, 0));
        //    painter.drawLine(_s, _e);
        //}
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

void Canvas::addFovy(int delta)
{
    this->camera->addFovy(delta);
    update();
}

void Canvas::setView(int index)
{
    this->camera->setView(index);
    update();
}

void Canvas::setShade(int index)
{
    this->_shade = index;
    update();
}
