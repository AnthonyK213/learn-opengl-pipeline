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

public slots:
    void changeFovy(int fovy);

protected:
    void paintEvent(QPaintEvent *event);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void draw();

private:
    float *z_buffer;

signals:

};

#endif // CANVAS_H
