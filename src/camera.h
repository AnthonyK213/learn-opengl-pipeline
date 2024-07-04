#ifndef CAMERA_H
#define CAMERA_H

#include <QMatrix4x4>

class Camera {
public:
  Camera();
  Camera(QMatrix4x4 &&theTrsf, int theFovy);
  QPointF Shot(QVector3D &v, float &z);
  QPointF Shot(QVector3D &&v, float &z);
  QPointF Shot(QVector3D &v, float &z, float &x, float &y);
  QPointF Shot(QVector3D &&v, float &z, float &x, float &t);
  void Transform(QMatrix4x4 &&trsf);
  const QMatrix4x4 &Transform() const;
  void SetFovy(int fovy);
  void AddFovy(int delta);
  void SetView(int index);

private:
  QMatrix4x4 myTrsf;
  QMatrix4x4 myViewMat;
  QMatrix4x4 myPerspMat;
  int myFovy;
  int myView = 0;
};

#endif // CAMERA_H
