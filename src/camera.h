#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera
{
public:
    Camera();
    Camera(QMatrix4x4&& tf, int&& fovy);
    ~Camera();
    QPointF shot(QVector3D &v, float& z);
    QPointF shot(QVector3D &&v, float& z);
    QPointF shot(QVector3D &v, float& z, float& x, float& y);
    QPointF shot(QVector3D &&v, float& z, float& x, float& t);
    void transform(QMatrix4x4&& tf);
    QMatrix4x4& tf();
    void setFovy(int fovy);
    void addFovy(int delta);
    void setView(int index);

private:
    QMatrix4x4 _tf;
    QMatrix4x4 _view_mat;
    QMatrix4x4 _persp_mat;
    int _fovy;
    int _view = 0;
};

#endif // CAMERA_H
