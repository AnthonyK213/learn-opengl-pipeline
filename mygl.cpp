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
    ui->cb_view->addItems(QStringList { "Perspective", "Parallel" });
    ui->cb_shade->addItems(QStringList { "Flat", "Smooth" });

    connect(ui->actionImport, &QAction::triggered, this, &Mygl::importPly);
    connect(ui->cb_view, &QComboBox::currentIndexChanged, ui->canvas, &Canvas::setView);
    connect(ui->cb_shade, &QComboBox::currentIndexChanged, ui->canvas, &Canvas::setShade);
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
    ui->canvas->setShadow();
    if (nullptr != bak)
        delete bak;
}

Model* Mygl::model() {
    return _model;
}
