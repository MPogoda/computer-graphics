#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDesktopWidget>
#include <QDebug>
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
    this->setWindowState(Qt::WindowFullScreen);
    this->setGeometry(QApplication::desktop()->geometry());
    buttons->addButton(ui->button1, 1);
    buttons->addButton(ui->button2, 2);
    buttons->addButton(ui->button3, 3);
    buttons->addButton(ui->button4, 4);
    ui->groupBox->setEnabled(false);
    connect(buttons, SIGNAL(buttonClicked(int)), this, SLOT(buttons_click(int)));
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
        const QPointF current = event->pos();
        const QPointF difference = current - origin;

        for (qint8 i = 0; i < 3; ++i)
            for (qint8 j = 0; j < 2; ++j)
                matrix[i][j] = 0.0;
        matrix[2][2] = 1.0;

        switch (state)
        {
        case MOVING:
            for (qint8 i = 0; i < 3; ++i)
                points[i] += difference;
            break;
        case SCALING:
            matrix[0][0] = 1.0 + difference.x() / 200.0;
            matrix[1][1] = 1.0 - difference.y() / 200.0;
            for (qint8 i = 0; i < 3; ++i)
            {
                points[i] -= real_origin;
                points[i] = mul(points[i]);
                points[i] += real_origin;
            }
            break;
        case ROTATING:
            matrix[0][0] = qCos(difference.x() / 50.0);
            matrix[1][1] = qCos(difference.x() / 50.0);
            matrix[0][1] = qSin(difference.x() / 50.0);
            matrix[1][0] = -qSin(difference.x() / 50.0);
            for (qint8 i = 0; i < 3; ++i)
            {
                points[i] -= real_origin;
                points[i] = mul(points[i]);
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

QPointF MainWindow::mul(const QPointF& point)
{
    static qreal vector[3];
    static qreal sum[3];
    vector[0] = point.x();
    vector[1] = point.y();
    vector[2] = 1.0;

    for (qint8 i = 0; i < 3; ++i)
    {
        sum[i] = 0.0;
        for (quint8 j = 0; j < 3; ++j)
            sum[i] += vector[j] * matrix[j][i];
    }

    return QPointF(sum[0], sum[1]);
}
