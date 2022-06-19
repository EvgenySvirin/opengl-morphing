#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "openglpicture.h"

MainWindow::MainWindow(QWidget *parent,
                       int t,
                       bool isAnimated):
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    t(t),
    isAnimated(isAnimated),
    colorDialog(new QColorDialog()) {
    ui->setupUi(this);
    colorDialog->setOption(QColorDialog::DontUseNativeDialog);
    auto validWidget = dynamic_cast<OpenGLPicture*>(ui->openGLWidget);
    if (validWidget) {
        ui->horizontalSlider->setRange(0, validWidget->getPeriodSize());
    }

    connectSignalsAndSlots();

    if (isAnimated) {
        ui->horizontalSlider->hide();
        validWidget->setAnimated(isAnimated);
    } else {
        validWidget->setChosenTime(t);
    }
}

MainWindow::~MainWindow() {
}

void MainWindow::connectSignalsAndSlots() {
    connect(this, SIGNAL(on_pushButton_clicked()),
            colorDialog.get(), SLOT(open()));
    connect(ui->lineEdit, SIGNAL(textChanged(QString)),
            ui->openGLWidget, SLOT(setRotationAxis(QString)));

    connect(this, SIGNAL(on_pushButton_3_clicked()),
            this, SLOT(changeAnimationState()));

    connect(colorDialog.get(), SIGNAL(colorSelected(QColor)),
            ui->openGLWidget, SLOT(setColor(QColor)));

    connect(ui->horizontalSlider, SIGNAL(valueChanged(int)),
            ui->openGLWidget, SLOT(setChosenTime(int)));
}


void MainWindow::changeAnimationState() {
    isAnimated = 1 - isAnimated;
    ui->openGLWidget->setAnimated(isAnimated);
    if (isAnimated) {
       ui->horizontalSlider->hide();
    } else {
       ui->horizontalSlider->show();
    }
}

