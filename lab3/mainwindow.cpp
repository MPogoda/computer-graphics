#include "mainwindow.h"
#include <QColorDialog>
#include <QQueue>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      clicked(false),
      pencolour(Qt::black),
      fillcolour(Qt::red),
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
        // Ask about colour for filling
        QColor newColour = QColorDialog::getColor(fillcolour, this);
        if (newColour.isValid())
            fillcolour = newColour;
        else
            return;

        fill(pixmap.toImage(), event->pos(), fillcolour.rgb());
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
        // Draw line between current and last known position
        p2 = event->pos();

        painter->setPen(pencolour);
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

void MainWindow::paintEvent(QPaintEvent *)
{
    label->setPixmap(pixmap);
}

// Fill 4-way linked area, determined by colour of start point
void MainWindow::fill(QImage img, QPoint p, QRgb fillcolour)
{
    const QRgb oldcolour = img.pixel(p);

    if (oldcolour == fillcolour) return;
    painter->setPen(fillcolour);

    QQueue<QPoint> queue;
    queue.enqueue(p);

    quint8 i = 0;
    while (!queue.isEmpty())
    {
        if (!++i)
            this->repaint();

        p = queue.dequeue();

        if ((p.x() + 1) < img.width())
        {
            QPoint tempPoint = QPoint(1, 0) + p;
            if (img.pixel(tempPoint) == oldcolour)
            {
                queue.enqueue(tempPoint);
                img.setPixel(tempPoint, fillcolour);
                painter->drawPoint(tempPoint);
            }
        }
        if (p.x() > 0)
        {
            QPoint tempPoint = QPoint(-1, 0) + p;
            if (img.pixel(tempPoint) == oldcolour)
            {
                queue.enqueue(tempPoint);
                img.setPixel(tempPoint, fillcolour);
                painter->drawPoint(tempPoint);
            }
        }
        if ((p.y() + 1) < img.height())
        {
            QPoint tempPoint = QPoint(0, 1) + p;
            if (img.pixel(tempPoint) == oldcolour)
            {
                queue.enqueue(tempPoint);
                img.setPixel(tempPoint, fillcolour);
                painter->drawPoint(tempPoint);
            }
        }
        if (p.y() > 0)
        {
            QPoint tempPoint = QPoint(0, -1) + p;
            if (img.pixel(tempPoint) == oldcolour)
            {
                queue.enqueue(tempPoint);
                img.setPixel(tempPoint, fillcolour);
                painter->drawPoint(tempPoint);
            }
        }
    }
}
