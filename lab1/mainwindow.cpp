#include "mainwindow.h"
#include <QColorDialog>
#include <QApplication>
#include <QDesktopWidget>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent),
      colour(Qt::black)
{
    this->setWindowState(Qt::WindowFullScreen);
    this->setGeometry(QApplication::desktop()->geometry());

    pixmap = QPixmap(this->size());
    pixmap.fill();
    painter = new QPainter(&pixmap);
}

MainWindow::~MainWindow()
{
    delete painter;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QColor newColour = QColorDialog::getColor(colour, this);
        if (newColour.isValid())
            colour = newColour;
    }
    else if (event->button() == Qt::LeftButton)
    {
        firstpoint = event->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        drawline(firstpoint, event->pos());
        this->update();
    }
}

void MainWindow::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    painter.drawPixmap(this->geometry(), pixmap);
}

void MainWindow::drawline(QPoint p1, QPoint p2)
{
    painter->setPen(colour);

    int x1     = p1.x();
    int y1     = p1.y();
    int x2     = p2.x();
    int y2     = p2.y();
    int deltaX = qAbs(x2 - x1);
    int deltaY = qAbs(y2 - y1);
    int signX  = (x1 < x2) ? 1 : -1;
    int signY  = (y1 < y2) ? 1 : -1;
    int d      = deltaX - deltaY;

    for (;;)
    {
        painter->drawPoint(x1, y1);

        if ((x1 == x2) && (y1 == y2))
            break;

        int d_next = d << 1;

        if (d_next < deltaX)
        {
            d  += deltaX;
            y1 += signY;
        }

        if (d_next > -deltaY)
        {
            d  -= deltaY;
            x1 += signX;
        }
    }
}
