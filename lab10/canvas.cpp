#include "canvas.h"
#include "math.h"
#include <QApplication>
#include <QBrush>
#include <QDebug>
#include <QPen>
#include <QPointF>
#include <iostream>

Canvas::Canvas(QWidget* parent)
    : QWidget(parent)
{
}

Canvas::~Canvas()
{
}

void Canvas::rotate_trans_matrix(matrix rotate_matrix)
{
    matrix res_matrix = { { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 }, { 0, 0, 0, 0 } };

    for (long i = 0; i < 4; i++)
        for (long j = 0; j < 4; j++)
            for (long k = 0; k < 4; k++)
                res_matrix[i][j] += trans_matrix[i][k] * rotate_matrix[k][j];

    for (long i = 0; i < 4; i++)
        for (long j = 0; j < 4; j++)
            trans_matrix[i][j] = res_matrix[i][j];
}

void Canvas::trans_point(point_3d& point)
{
    point.push_back(1);
    point_3d res_point(4);
    for (long i = 0; i < 4; i++)
        for (long j = 0; j < 4; j++)
            res_point[i] += point[j] * trans_matrix[j][i];

    for (long i = 0; i < 3; i++)
        res_point[i] *= scale;

    res_point[0] += field.width() / 2;
    res_point[1] += field.height() / 2;

    point[0] = res_point[0];
    point[1] = res_point[1];
    point[2] = res_point[2];
}

void Canvas::rotate_x(double angle)
{
    double value = angle / 180 * M_PI;
    matrix rotate_matrix = { { 1, 0, 0, 0 },
        { 0, cos(value), sin(value), 0 },
        { 0, -sin(value), cos(value), 0 },
        { 0, 0, 0, 1 } };
    rotate_trans_matrix(rotate_matrix);
}

void Canvas::rotate_y(double angle)
{
    double value = angle / 180 * M_PI;
    matrix rotate_matrix = { { cos(value), 0, -sin(value), 0 },
        { 0, 1, 0, 0 },
        { sin(value), 0, cos(value), 0 },
        { 0, 0, 0, 1 } };
    rotate_trans_matrix(rotate_matrix);
}

void Canvas::rotate_z(double angle)
{
    double value = angle / 180 * M_PI;
    matrix rotate_matrix = { { cos(value), sin(value), 0, 0 },
        { -sin(value), cos(value), 0, 0 },
        { 0, 0, 1, 0 },
        { 0, 0, 0, 1 } };
    rotate_trans_matrix(rotate_matrix);
}

void Canvas::set_scale(double sc)
{
    scale = sc;
}

bool Canvas::is_visible(double x, double y)
{
    return 0 <= x && x < field.width() && 0 <= y && y < field.height();
}

bool Canvas::draw_point(double x, double y, vector<double>& hh, vector<double>& lh)
{
    painter.setPen(QPen(fg));

    if (not is_visible(x, y))
        return false;

    if (y > hh[x]) {
        hh[x] = y;
        painter.drawPoint(x, y);
    } else if (y < lh[x]) {
        lh[x] = y;
        painter.drawPoint(x, y);
    }

    return true;
}

void Canvas::draw_horizon_part(point_3d p1, point_3d p2, vector<double>& hh, vector<double>& lh)
{
    if (p1[0] > p2[0]) {
        point_3d tmp = p1;
        p1 = p2;
        p2 = tmp;
    }

    double dx = p2[0] - p1[0];
    double dy = p2[1] - p1[1];
    double l = max(dx, dy);
    dx /= l;
    dy /= l;

    double x = p1[0], y = p1[1];

    for (long i = 0; i < long(l) + 1; i++) {
        if (not draw_point(int(round(x)), y, hh, lh))
            return;
        x += dx;
        y += dy;
    }
}

void Canvas::draw_horizon(double (*func)(double, double), vector<double>& hh, vector<double>& lh, double fr, double to, double step, double z)
{
    point_3d prev;
    //    qDebug() << fr << to << step << z;
    for (double x = fr; x < to + step; x += step) {
        point_3d current({ x, func(x, z), z });
        trans_point(current);
        if (prev.size())
            draw_horizon_part(prev, current, hh, lh);
        prev = current;
    }
}

void Canvas::draw(double (*f)(double x, double z), double x_min, double x_max, double x_step, double z_min, double z_max, double z_step)
{
    image = QImage(this->width(), this->height(), QImage::Format_RGB32);
    image.fill(bg);
    painter.begin(&image);
    field = geometry();
    painter.setPen(QPen(fg));
    vector<double> high_horizon;
    for (long i = 0; i < field.width(); i++)
        high_horizon.push_back(0);
    vector<double> low_horizon;
    for (long i = 0; i < field.width(); i++)
        low_horizon.push_back(field.height());

    for (double z = z_min; z < z_max + z_step; z += z_step)
        draw_horizon(f, high_horizon, low_horizon, x_min, x_max, x_step, z);

    for (double z = z_min; z < z_max; z += z_step) {
        point_3d p1({ x_min, f(x_min, z), z }), p2({ x_min, f(x_min, z + z_step), z + z_step });
        trans_point(p1);
        trans_point(p2);
        painter.drawLine(p1[0], p1[1], p2[0], p2[1]);

        point_3d p3({ x_max, f(x_max, z), z }), p4({ x_max, f(x_max, z + z_step), z + z_step });
        trans_point(p3);
        trans_point(p4);
        painter.drawLine(p3[0], p3[1], p4[0], p4[1]);
    }
    this->update();
    painter.end();
}

void Canvas::set_bg(const QColor& color)
{
    bg = color;
    this->update();
}

void Canvas::set_fg(const QColor& color)
{
    fg = color;
    this->update();
}

void Canvas::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    QPainter painter_whole(this);

    painter_whole.drawImage(QPoint(), image);
}

void Canvas::clear()
{
    image.fill(bg);
    for (long i = 0; i < 4; i++)
        for (long j = 0; j < 4; j++)
            if (i == j)
                trans_matrix[i][j] = 1;
            else
                trans_matrix[i][j] = 0;
    scale = 100;
    this->update();
}
