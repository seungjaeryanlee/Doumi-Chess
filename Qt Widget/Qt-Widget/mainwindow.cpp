#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QtWidgets>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(update()));
    timer->start(1000);

    showTime();
}
MainWindow::~MainWindow()
{
    delete ui;
}
void MainWindow::showTime()
{
    QTime time = QTime::currentTime();
    QString text = time.toString("mm:ss");
    qDebug() << text;
}

