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

const QPolygon& Widget::do_proection(const QVector<QVector3D> &ps) const
{
    static QPolygon result;

    result.clear();
    foreach (const QVector3D& point, ps)
    {
        static QPoint proected_point;
        proected_point = (point.toPoint() - origin) * 200 / (point.z() + z) ;
        proected_point.ry() *= -1;
        proected_point += shift;

        result << proected_point;
    }

    return result;
}

void Widget::draw_figure()
{
    draw_face(QVector<QVector3D>(0) << points[0] << points[1] << points[2] << points[3]);
    draw_face(QVector<QVector3D>(0) << points[0] << points[1] << points[6] << points[7]);
    draw_face(QVector<QVector3D>(0) << points[1] << points[2] << points[5] << points[6]);
    draw_face(QVector<QVector3D>(0) << points[2] << points[3] << points[4] << points[5]);
    draw_face(QVector<QVector3D>(0) << points[0] << points[3] << points[4] << points[7]);

    painter->drawText(900, 0, 124, 20, 0,
                      "(" + QString::number(origin.x()) + ", "
                          + QString::number(origin.y()) + ", "
                          + QString::number(-z) + ")");

    this->update();
}

void Widget::draw_face(const QVector<QVector3D> &ps)
{
    const QPolygon& proected_points = do_proection(ps);

    painter->drawPolygon(proected_points);
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
    static QPoint tmp;

    switch (mouse_button)
    {
    case LEFT_BUTTON:
        tmp = (event->pos() - last_known_position);
        tmp.ry() *= -1;
        origin -= tmp;
        break;
    case RIGHT_BUTTON:
        z -= (event->pos().x() - last_known_position.x());
        if (z < 60)
            z = 60;
    case NOP:
        break;
    }

    last_known_position = event->pos();

    pixmap.fill();

    draw_figure();
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
