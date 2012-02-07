#include "mainwindow.h"
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      color(Qt::black),
      pixmap(QPixmap(1024, 600)),
      dialog(new QColorDialog(color, this)),
      label(new QLabel(this))
{
    this->setGeometry(QRect(0, 0, 1024, 600));
    label->setGeometry(this->geometry());
    pixmap.fill();
}

MainWindow::~MainWindow()
{
    delete dialog;
    delete label;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QColor newColor = dialog->getColor(color, this);
        if (newColor.isValid())
            color = newColor;
    }
    else if (event->button() == Qt::LeftButton)
    {
        p1 = event->pos();
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        p2 = event->pos();
        drawline(p1, p2);
        this->update();
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    label->setPixmap(pixmap);
}

void MainWindow::drawline(QPoint p1, QPoint p2)
{
    QPainter painter(&pixmap);
    painter.setPen(color);

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
        painter.drawPoint(x1, y1);

        if ((x1 == x2) && (y1 == y2))
            break;

        int d_next = d << 1;

        if (d_next > -deltaY)
        {
            d  -= deltaY;
            x1 += signX;
        }

        if (d_next < deltaX)
        {
            d  += deltaX;
            y1 += signY;
        }
    }

}
