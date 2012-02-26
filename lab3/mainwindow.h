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
    void mouseMoveEvent(QMouseEvent *);
    void fill(QImage, QPoint, QRgb);

    bool           clicked;
    const QColor   pencolour;
    QColor         fillcolour;
    QPoint         p1;
    QPoint         p2;
    QPixmap        pixmap;
    QPainter       *painter;
    QPainter       *wpainter;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
