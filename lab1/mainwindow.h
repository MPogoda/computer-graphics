#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtGui/QMainWindow>
#include <QMouseEvent>
#include <QPaintDevice>
#include <QPainter>
#include <QColor>
#include <QColorDialog>

class MainWindow : public QMainWindow
{
    Q_OBJECT
    void mousePressEvent(QMouseEvent *);
    void paintEvent(QPaintEvent *);
    bool clicked;
    bool firstrun;
    QColor color;
    QColorDialog *dialog;
    int x1;
    int y1;
    int x2;
    int y2;
public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();
};

#endif // MAINWINDOW_H
