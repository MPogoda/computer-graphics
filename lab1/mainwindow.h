#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QColor>
#include <QPixmap>

class MainWindow : public QWidget
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawline(QPoint, QPoint);

    QColor   colour;
    QPoint   firstpoint;
    QPixmap  pixmap;
    QPainter *painter;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
