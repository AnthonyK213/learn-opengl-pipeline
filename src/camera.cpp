#include "camera.h"

Camera::Camera() {}

Camera::Camera(QMatrix4x4 &&theTrsf, int theFovy) : myTrsf(theTrsf) {
  SetFovy(theFovy);
  myViewMat = theTrsf.inverted();
}

QPointF Camera::Shot(QVector3D &v, float &z) {
  QVector3D r = myViewMat.map(v);
  z = r.z();
  if (myView == 0)
    r = myPerspMat.map(r);
  return QPointF((r.x() + 1.) * 690 / 2, (r.y() + 1.) * 690 / 2);
}

QPointF Camera::Shot(QVector3D &&v, float &z) {
  QVector3D r = myViewMat.map(v);
  z = r.z();
  if (myView == 0) {
    r = myPerspMat.map(r);
  }
  return QPointF((r.x() + 1.) * 690 / 2, (r.y() + 1.) * 690 / 2);
}

QPointF Camera::Shot(QVector3D &v, float &z, float &x, float &y) {
  QVector3D r = myViewMat.map(v);
  z = r.z();
  x = r.x();
  y = r.y();
  if (myView == 0)
    r = myPerspMat.map(r);
  return QPointF((r.x() + 1.) * 690 / 2, (r.y() + 1.) * 690 / 2);
}

QPointF Camera::Shot(QVector3D &&v, float &z, float &x, float &y) {
  QVector3D r = myViewMat.map(v);
  z = r.z();
  x = r.x();
  y = r.y();
  if (myView == 0) {
    r = myPerspMat.map(r);
  }
  return QPointF((r.x() + 1.) * 690 / 2, (r.y() + 1.) * 690 / 2);
}

void Camera::Transform(QMatrix4x4 &&tf) {
  myTrsf = tf * myTrsf;
  myViewMat = myTrsf.inverted();
}

const QMatrix4x4 &Camera::Transform() const { return myTrsf; }

void Camera::SetFovy(int fovy) {
  myFovy = fovy;
  float a = 1. / qTan(0.5 * qDegreesToRadians(myFovy));
  myPerspMat = QMatrix4x4(a, 0, 0, 0, 0, a, 0, 0, 0, 0, -1, 0, 0, 0, -1, 0);
}

void Camera::AddFovy(int delta) {
  int fovy = qMin(qMax(10, myFovy + delta), 169);
  SetFovy(fovy);
}

void Camera::SetView(int index) { myView = index; }
