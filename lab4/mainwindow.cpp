#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <qmath.h>

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow),
    buttons(new QButtonGroup(this)),
    mouse_clicked(false),
    just_started(true),
    state(SELECTING),
    points_stored(0),
    fgcolour(Qt::red),
    bgcolour(Qt::white)
{
    ui->setupUi(this);

    this->setGeometry(QApplication::desktop()->geometry());
    this->setWindowState(Qt::WindowFullScreen);

    buttons->addButton(ui->button1, 1);
    buttons->addButton(ui->button2, 2);
    buttons->addButton(ui->button3, 3);
    buttons->addButton(ui->button4, 4);
    connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(buttons_click(int)));
    ui->groupBox->setEnabled(false);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete painter;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    if (just_started)
    {
        pixmap = QPixmap(ui->label->size());
        pixmap.fill(bgcolour);
        painter = new QPainter(&pixmap);
        just_started = false;
    }

    ui->label->setPixmap(pixmap);
}

void MainWindow::buttons_click(int number)
{
    switch (number)
    {
    case 1: state = SELECTING; break;
    case 2: state = MOVING;    break;
    case 3: state = SCALING;   break;
    case 4: state = ROTATING;  break;
    }
}

void MainWindow::mousePressEvent(QMouseEvent *event)
{
    const QPointF event_pos = event->pos() - ui->label->pos();

    switch (state)
    {
    case SELECTING:
        switch (points_stored)
        {
        case 3:
            drawTriangle(points, bgcolour);
            points_stored = 0;
        case 0:
        case 1:
            points[ points_stored++ ] = event_pos;

            ui->groupBox->setEnabled(false);
            break;
        case 2:
            points[ points_stored++ ] = event_pos;
            drawTriangle(points, fgcolour);

            ui->groupBox->setEnabled(true);
            break;
        }
        break;
    default:
        mouse_clicked = true;
        origin = event->pos();
        real_origin = origin;
    }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *)
{
    if (state != SELECTING)
    {
        mouse_clicked = false;
    }
}

void MainWindow::mouseMoveEvent(QMouseEvent *event)
{
    if ((state != SELECTING) && mouse_clicked)
    {
        drawTriangle(points, bgcolour);
        const QPointF current    = event->pos();
        const QPointF difference = current - origin;

        if (state == MOVING)
        {
            for (qint8 i = 0; i < 3; ++i)
            {
                points[i] += difference;
            }
        }
        else if (state == SCALING)
        {
            for (qint8 i = 0; i < 3; ++i)
            {
                points[i] -= real_origin;
                points[i].rx() *= 1.0 + difference.x() / 200.0;
                points[i].ry() *= 1.0 - difference.y() / 200.0;
                points[i] += real_origin;
            }
        }
        else
        {
            const qreal cosinus = qCos(difference.x() / 50.0);
            const qreal sinus   = qSin(difference.x() / 50.0);
            for (qint8 i = 0; i < 3; ++i)
            {
                points[i] -= real_origin;
                points[i] = QPointF(points[i].x() * cosinus - points[i].y() * sinus,
                                    points[i].x() * sinus + points[i].y() * cosinus);
                points[i] += real_origin;
            }
        }

        origin = current;
        drawTriangle(points, fgcolour);
    }
}

void MainWindow::drawTriangle(const QPointF *points, const QColor& colour)
{
    painter->setPen(colour);
    painter->drawLine(points[0], points[1]);
    painter->drawLine(points[1], points[2]);
    painter->drawLine(points[2], points[0]);

    this->update();
}
