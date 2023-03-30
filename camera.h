#ifndef CAMERA_H
#define CAMERA_H

#define ACCURATE_PERSPECTIVE 0

#include <QMatrix4x4>
#include "geometry.h"

class Camera
{
public:
    Camera();
    Camera(QMatrix4x4&& tf, int&& fovy);
    ~Camera();
    QPointF shot(vec3 &v, float& z);
    QPointF shot(vec3 &&v, float& z);
    void transform(QMatrix4x4&& tf);
    QMatrix4x4& tf();
    void setFovy(int fovy);
    void addFovy(int delta);
    void setView(int index);

private:
    QMatrix4x4 _tf;
    QMatrix4x4 _tf_inv;
    QMatrix4x4 _persp;
    int _fovy;
    int _view = 0;
};

#endif // CAMERA_H
