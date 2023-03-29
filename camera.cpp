#include "camera.h"

Camera::Camera() {}

Camera::Camera(QMatrix4x4&& tf, int&& fovy) : _tf(tf)
{
    setFovy(fovy);
    this->_tf_inv = tf.inverted();
}

Camera::~Camera() {}

QPointF Camera::shot(vec3 &v, float& z)
{
    QVector3D t = this->_tf_inv.map(QVector3D(v.x, v.y, v.z));
    QVector3D r = this->_persp.map(t);
    z = t.z();
    return QPointF((r.x() + 1.) * 500, (r.y() + 1.) * 500);
}

QPointF Camera::shot(vec3 &&v, float& z)
{
    QVector3D r = this->_tf_inv.map(QVector3D(v.x, v.y, v.z));
    z = r.z();
    if (this->_view == 0) r = this->_persp.map(r);
    return QPointF((r.x() + 1.) * 500, (r.y() + 1.) * 500);
}

void Camera::transform(QMatrix4x4&& tf)
{
    this->_tf = tf * this->_tf;
    this->_tf_inv = this->_tf.inverted();
}

QMatrix4x4& Camera::tf()
{
    return this->_tf;
}

void Camera::setFovy(int fovy)
{
    this->_fovy = fovy;
    //this->_persp = QMatrix4x4();
    //this->_persp.perspective((float)fovy, 1., 0, 100);
    float a = 1. / qTan(0.5 * qDegreesToRadians(_fovy));
    this->_persp = QMatrix4x4(a, 0, 0, 0, 0, a, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0);
}

void Camera::addFovy(int delta)
{
    int fovy = qMin(qMax(10, this->_fovy + delta), 169);
    setFovy(fovy);
}

void Camera::setView(int index)
{
    this->_view = index;
}
