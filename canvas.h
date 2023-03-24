#ifndef CANVAS_H
#define CANVAS_H

#include <QObject>
#include <QWidget>
#include <QPainter>
#include <QDebug>
#include <QMouseEvent>
#include "model.h"

class Canvas : public QWidget
{
    Q_OBJECT
public:
    explicit Canvas(QWidget *parent = nullptr);

    QImage *temp;
    quint32 *pic;
    int Move_flag;
    int x, y;
    int x_old, y_old;
    QImage *image;

protected:
    void paintEvent(QPaintEvent *event);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void draw();

signals:

};

static QPointF vec3ToQPointF(vec3 v, QMatrix4x4& tf);

#endif // CANVAS_H
