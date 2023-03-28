#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>
#include "mygl.h"
#include "./ui_mygl.h"
#include "model.h"
#include "canvas.h"

Mygl::Mygl(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::Mygl)
    , _model(nullptr)
{
    ui->setupUi(this);

    connect(ui->actionImport, &QAction::triggered, this, &Mygl::importPly);
    connect(ui->horizontalSlider, &QSlider::sliderMoved, ui->widget, &Canvas::changeFovy);
}

Mygl::~Mygl()
{
    delete _model;
    delete ui;
}

void Mygl::importPly()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Open a file");
    if (fileName.isEmpty())
    {
        return;
    }
    Model* bak = this->_model;
    this->_model = new Model(fileName.toStdString());
    if (nullptr != bak)
        delete bak;
}

Model* Mygl::model() {
    return _model;
}
