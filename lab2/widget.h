#ifndef WIDGET_H
#define WIDGET_H

#include <QtGui/QWidget>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QPixmap>
#include <QVector>
#include <QPoint>

class Widget : public QWidget
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent *);
    void keyPressEvent(QKeyEvent *);
    void paintEvent(QPaintEvent *);
    void drawSplines();

    QPainter        *painter;
    QPixmap         pixmap;
    QVector<QPoint> points;
public:
    Widget(QWidget *parent = 0);
    ~Widget();
};

#endif // WIDGET_H
