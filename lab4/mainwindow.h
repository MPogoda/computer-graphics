#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QWidget>
#include <QButtonGroup>
#include <QPainter>
#include <QMouseEvent>

namespace Ui {
class MainWindow;
}

class MainWindow : public QWidget
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    QButtonGroup   *buttons;

    bool            mouse_clicked;
    bool            just_started;
    enum { SELECTING, MOVING,
           SCALING, ROTATING } state;
    QPixmap         pixmap;
    QPainter        *painter;
    QPointF         points[3];
    quint8          points_stored;
    QPointF         origin;
    QPointF         real_origin;
    const QColor    fgcolour;
    const QColor    bgcolour;

    void paintEvent(QPaintEvent *);
    void mousePressEvent(QMouseEvent *);
    void mouseReleaseEvent(QMouseEvent *);
    void mouseMoveEvent(QMouseEvent *);
    void drawTriangle(const QPointF*, const QColor &);
private slots:
    void buttons_click(int);
};

#endif // MAINWINDOW_H
