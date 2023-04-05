#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "camera.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);
    ~Canvas();

    int _move_flag;
    int x, y;
    int x_old, y_old;
    QImage *image;
    Camera *camera;
    Camera *light;

public slots:
    void changeFovy(int fovy);
    void setView(int index);
    void setShade(int index);
    void setShadow();

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void wheelEvent(QWheelEvent *e);
    void draw();
    void addFovy(int delta);

private:
    float *z_buffer;
    float *shadow_buffer;
    int _shade = 0;

signals:

};

#endif // CANVAS_H
