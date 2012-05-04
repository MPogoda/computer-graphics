#include "widget.h"
#include <QApplication>
#include <QDesktopWidget>

Widget::Widget(QWidget *parent)
    : QWidget(parent),
      mouse_button(NOP),
      foreground(Qt::blue),
      background(Qt::white)
{
    this->setGeometry(QApplication::desktop()->geometry());
    this->setWindowState(Qt::WindowFullScreen);

    init_figure();
    shift = QPoint(this->size().width() >> 1, this->size().height() >> 1);

    pixmap = QPixmap(this->size());
    pixmap.fill();
    painter = new QPainter(&pixmap);

    painter->setPen(foreground);
    do_proection();
    draw_figure();
}

void Widget::init_figure()
{
    points[0] = QVector3D( 100,  80, 00);
    points[1] = QVector3D( 100, -80, 00);
    points[2] = QVector3D(-100, -80, 00);
    points[3] = QVector3D(-100,  80, 00);
    points[4] = QVector3D(-100,  80, 60);
    points[5] = QVector3D(-100, -80, 60);
    points[6] = QVector3D( 100, -80, 60);
    points[7] = QVector3D( 100,  80, 60);

    origin = QPoint(0, 0);
    z = 200;
}

void Widget::do_proection()
{
    for (qint8 i = 0; i < 8; ++i)
    {
        proected_points[i] = points[i].toPoint() - origin;
        proected_points[i] *= 200 / (points[i].z() + z);
        proected_points[i] += origin + shift;
    }
}

void Widget::draw_figure()
{
    painter->drawLine(proected_points[0], proected_points[1]);
    painter->drawLine(proected_points[0], proected_points[7]);
    painter->drawLine(proected_points[0], proected_points[3]);

    painter->drawLine(proected_points[1], proected_points[2]);
    painter->drawLine(proected_points[1], proected_points[6]);

    painter->drawLine(proected_points[2], proected_points[5]);
    painter->drawLine(proected_points[2], proected_points[3]);

    painter->drawLine(proected_points[3], proected_points[4]);

    painter->drawLine(proected_points[4], proected_points[5]);
    painter->drawLine(proected_points[4], proected_points[7]);

    painter->drawLine(proected_points[5], proected_points[6]);

    painter->drawLine(proected_points[6], proected_points[7]);
}

Widget::~Widget()
{
    delete painter;
}

void Widget::mousePressEvent(QMouseEvent *event)
{
    switch (event->button())
    {
    case Qt::LeftButton:
        mouse_button = LEFT_BUTTON;
        break;
    case Qt::RightButton:
        mouse_button = RIGHT_BUTTON;
        break;
    default:
        mouse_button = NOP;
    }

    last_known_position = event->pos();
}

void Widget::mouseMoveEvent(QMouseEvent *event)
{
    switch (mouse_button)
    {
    case LEFT_BUTTON:
        origin -= (event->pos() - last_known_position);
        break;
    case RIGHT_BUTTON:
        z -= (event->pos().x() - last_known_position.x());
        if (z < 60)
            z = 60;
    case NOP:
        break;
    }

    last_known_position = event->pos();

    painter->setPen(background);
    draw_figure();
    painter->fillRect(900, 0, 124, 20, background);
    painter->setPen(foreground);

    do_proection();
    draw_figure();
    painter->drawText(900, 0, 124, 20, 0,
                      "(" + QString::number(origin.x()) + ", "
                          + QString::number(-origin.y()) + ", "
                          + QString::number(z) + ")");

    this->update();

}

void Widget::mouseReleaseEvent(QMouseEvent *)
{
    mouse_button = NOP;
}

void Widget::keyPressEvent(QKeyEvent *event)
{
    switch (event->key())
    {
    case Qt::Key_Return:
        init_figure();
        draw_figure();
        break;
    case Qt::Key_Escape:
        this->close();
    }
}

void Widget::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    painter.drawPixmap(event->rect(), pixmap, event->rect());
}
