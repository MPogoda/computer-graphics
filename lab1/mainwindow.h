#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QColor>
#include <QPixmap>
#include <QLabel>

class MainWindow : public QMainWindow
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
    QLabel   *label;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
