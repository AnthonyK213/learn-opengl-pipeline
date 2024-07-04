#ifndef CANVAS_H
#define CANVAS_H

#include <QDebug>
#include <QMouseEvent>
#include <QObject>
#include <QPainter>
#include <QWidget>

class Camera;

class Canvas : public QWidget {
  Q_OBJECT

public:
  explicit Canvas(QWidget *parent = nullptr);
  ~Canvas();

public slots:
  void ChangeFovy(int fovy);
  void SetView(int index);
  void SetShade(int index);
  void SetShadow();

protected:
  virtual void paintEvent(QPaintEvent *e) override;
  virtual void mouseMoveEvent(QMouseEvent *e) override;
  virtual void mouseReleaseEvent(QMouseEvent *e) override;
  virtual void wheelEvent(QWheelEvent *e) override;
  void draw();
  void addFovy(int delta);

private:
  QImage *myImage;
  Camera *myCamera;
  Camera *myLight;
  float *myZBuf;
  float *myShadowBuf;
  int myShade = 0;
  int myMoveFlag;
  int myX, myY;
  int myOldX, myOldY;
};

#endif // CANVAS_H
