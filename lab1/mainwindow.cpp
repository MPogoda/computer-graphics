#include "mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),
      clicked(false),
      firstrun(true),
      color(Qt::black),
      dialog(new QColorDialog(color, this))
{
    this->setGeometry(QRect(0, 0, 1024, 600));
}

MainWindow::~MainWindow()
{
    delete dialog;
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::RightButton)
    {
        QColor newColor = dialog->getColor(color, this);
        if (newColor.isValid())
            color = newColor;
    }
    else if (clicked)
    {
        x2 = event->pos().x();
        y2 = event->pos().y();
        clicked = false;
        this->repaint(); // CALL paintEVENT
    }
    else
    {
        x1 = event->pos().x();
        y1 = event->pos().y();
        clicked = true;
    }
}

void MainWindow::paintEvent(QPaintEvent *event)
{
    if (clicked)
        return;
    if (firstrun)
    {
        firstrun = false;
        return;
    }

    QPainter painter(this);
    painter.setPen(color);

    int deltaX  = qAbs(x2 - x1);
    int deltaY  = qAbs(y2 - y1);
    int signX   = (x1 < x2) ? 1 : -1;
    int signY   = (y1 < y2) ? 1 : -1;
    int error   = deltaX - deltaY;

    for (;;)
    {
        painter.drawPoint(x1, y1);

        if ((x1 == x2) && (y1 == y2))
            break;

        int error2 = error << 1;

        if (error2 > -deltaY)
        {
            error   -= deltaY;
            x1      += signX;
        }

        if (error2 < deltaX)
        {
            error   += deltaX;
            y1      += signY;
        }
    }
}
