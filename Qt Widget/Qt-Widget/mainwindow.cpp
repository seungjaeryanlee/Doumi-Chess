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
void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPointF textPosition;
    textPosition.setX(100);
    textPosition.setY(100);
    QFont font = painter.font();
    font.setPointSize(20);
    painter.setFont(font);
    painter.drawText(textPosition, "Time");
}
