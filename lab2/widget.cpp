#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QColor>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
{
    this->setGeometry(QApplication::desktop()->geometry());
    this->setWindowState(Qt::WindowFullScreen);

    pixmap = QPixmap(this->size());
    pixmap.fill();
    painter = new QPainter(&pixmap);
}

Widget::~Widget()
{
    delete painter;
}

void Widget::drawSplines()
{
    if (points.size() < 4)
        return;

    points.push_front(points.last());
    points << points.at(1);
    points << points.at(2);

    qreal a3, a2, a1, a0, b3, b2, b1, b0;

    quint8 i = 0;
    while (points.size() - 3 > i)
    {
        const QPoint &p0 = points.at(i);
        const QPoint &p1 = points.at(i+1);
        const QPoint &p2 = points.at(i+2);
        const QPoint &p3 = points.at(i+3);

        a3 = (-p0.x() + 3.0 * p1.x() - 3.0 * p2.x() + p3.x()) / 6.0;
        a2 = (p0.x() - 2.0 * p1.x() + p2.x()) / 2.0;
        a1 = (-p0.x() + p2.x()) / 2.0;
        a0 = (p0.x() + 4.0 * p1.x() + p2.x()) / 6.0;

        b3 = (-p0.y() + 3.0 * p1.y() - 3.0 * p2.y() + p3.y()) / 6.0;
        b2 = (p0.y() - 2.0 * p1.y() + p2.y()) / 2.0;
        b1 = (-p0.y() + p2.y()) / 2.0;
        b0 = (p0.y() + 4.0 * p1.y() + p2.y()) / 6.0;

        const qreal step = 0.0004;
        qreal t = 0.0;
        qint32 x, y;
        qint8 j = 0;

        painter->setPen(QColor::fromRgb(qAbs(qRound(a3+a2+a1+a0)) % 256,
                                        qAbs(qRound(b3+b2+b1+b0)) % 256,
                                        qAbs(qRound(a3+a2+b1+b0)) % 256));
        while (t <= 1.0)
        {
            x = ((a3 * t + a2) * t + a1) * t + a0;
            y = ((b3 * t + b2) * t + b1) * t + b0;

            painter->drawPoint(x, y);

            t += step;

            if (++j % 64 == 0)
                this->repaint();
        }
        ++i;
    }

    points.pop_back(); points.pop_back(); points.pop_front();
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        points << event->pos();

        painter->setPen(Qt::black);
        painter->drawRect(event->pos().x() - 1, event->pos().y() - 1, 2, 2);
        break;
    case Qt::RightButton:
        pixmap.fill();
        foreach (QPoint point, points)
        {
            painter->drawRect(point.x() - 1, point.y() - 1, 2, 2);
        }

        drawSplines();
    }

    this->update();
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
        pixmap.fill();
        points.clear();
        this->update();

        break;
    case Qt::Key_Escape:
        this->close();
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(this->geometry(), pixmap);
}
