#ifndef MYGL_H
#define MYGL_H

#include <QMainWindow>
#include "model.h"

QT_BEGIN_NAMESPACE
namespace Ui { class Mygl; }
QT_END_NAMESPACE

class Mygl : public QMainWindow
{
    Q_OBJECT

public:
    Mygl(QWidget *parent = nullptr);
    ~Mygl();
    Model* model();

private slots:
    void importPly();

private:
    Ui::Mygl *ui;
    Model *_model;
    void redraw();
    void drawLine();

    QPixmap *pixmap;
    bool pressed;
};
#endif // MYGL_H
