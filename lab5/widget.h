#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QPixmap>
#include <QVector3D>
#include <QPoint>

class Widget : public QWidget
{
    Q_OBJECT
    void keyPressEvent(QKeyEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);

    void init_figure();
    void do_proection();
    void draw_figure();

    const static quint8 NUMBER_OF_VERTEX = 8;

    QPixmap   pixmap;
    QPainter  *painter;

    QVector3D points[NUMBER_OF_VERTEX];
    QPoint    proected_points[NUMBER_OF_VERTEX];
    QPoint    origin;
    QPoint    shift;
    qint32    z;
    QPoint    last_known_position;
    enum { NOP, LEFT_BUTTON, RIGHT_BUTTON }
         mouse_button;

    QColor    foreground;
    QColor    background;

public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
