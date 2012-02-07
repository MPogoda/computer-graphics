#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QColor>
#include <QColorDialog>
#include <QPixmap>
#include <QLabel>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    void drawline(QPoint, QPoint);

    bool         clicked;
    QColor       color;
    QPoint       p1;
    QPoint       p2;
    QPixmap      pixmap;
    QColorDialog *dialog;
    QLabel       *label;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
