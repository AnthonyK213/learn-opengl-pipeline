#include "mygl.h"
#include "./ui_mygl.h"
#include "canvas.h"
#include "model.h"

#include <QFile>
#include <QFileDialog>
#include <QMessageBox>
#include <QPainter>

Mygl::Mygl(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::Mygl), myModel(nullptr) {
  ui->setupUi(this);
  ui->cb_view->addItems(QStringList{"Perspective", "Parallel"});
  ui->cb_shade->addItems(QStringList{"Flat", "Smooth"});

  connect(ui->actionImport, &QAction::triggered, this, &Mygl::importPly);
  connect(ui->cb_view, &QComboBox::currentIndexChanged, ui->canvas,
          &Canvas::SetView);
  connect(ui->cb_shade, &QComboBox::currentIndexChanged, ui->canvas,
          &Canvas::SetShade);
}

Mygl::~Mygl() {
  delete myModel;
  delete ui;
}

void Mygl::importPly() {
  QString fileName = QFileDialog::getOpenFileName(this, "Open a file");
  if (fileName.isEmpty()) {
    return;
  }
  Model *bak = this->myModel;
  this->myModel = new Model(fileName.toStdString());
  ui->canvas->SetShadow();
  if (nullptr != bak)
    delete bak;
}

Model *Mygl::model() { return myModel; }
