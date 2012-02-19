#include "mainwindow.h"
#include <QColorDialog>
#include <QStack>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      clicked(false),
      pencolor(Qt::black),
      fillcolor(Qt::red),
      pixmap(QPixmap(1024, 600)),
      label(new QLabel(this)),
      painter(new QPainter(&pixmap))
{
    this->setGeometry(QRect(0, 0, 1024, 600));
    label->setGeometry(this->geometry());
    pixmap.fill();
}

MainWindow::~MainWindow()
{
    delete painter;
    delete label;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QImage img = pixmap.toImage();
        QColor newColor = QColorDialog::getColor(fillcolor, this);
        if (newColor.isValid())
            fillcolor = newColor;
        else
            return;
        fill(img, event->pos(), fillcolor.rgb());
        delete painter;
        pixmap = QPixmap::fromImage(img);
        painter = new QPainter(&pixmap);
        this->update();

    }
    else if (event->button() == Qt::LeftButton)
    {
        clicked = true;
        p1 = event->pos();
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (clicked)
    {
        p2 = event->pos();

        painter->setPen(pencolor);
        painter->drawLine(p1, p2);

        p1 = p2;
        this->update();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        clicked = false;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    label->setPixmap(pixmap);
}

void MainWindow::fill(QImage& img, QPoint p, QRgb fillcolor)
{
    const QRgb oldcolor = img.pixel(p);

    if (oldcolor == fillcolor) return;

    QStack<QPoint> stack;
    stack.push(p);

    while (!stack.isEmpty())
    {
        p = stack.pop();
        img.setPixel(p, fillcolor);

        if ((p.x() + 1) < img.width())
        {
            QPoint tempPoint = QPoint(1, 0);
            tempPoint += p;
            if (img.pixel(tempPoint) == oldcolor) stack.push(tempPoint);
        }
        if (p.x() > 0)
        {
            QPoint tempPoint = QPoint(-1, 0);
            tempPoint += p;
            if (img.pixel(tempPoint) == oldcolor) stack.push(tempPoint);
        }
        if ((p.y() + 1) < img.height())
        {
            QPoint tempPoint = QPoint(0, 1);
            tempPoint += p;
            if (img.pixel(tempPoint) == oldcolor) stack.push(tempPoint);
        }
        if (p.y() > 0)
        {
            QPoint tempPoint = QPoint(0, -1);
            tempPoint += p;
            if (img.pixel(tempPoint) == oldcolor) stack.push(tempPoint);
        }
    }
}
