#ifndef CANVAS_H
#define CANVAS_H

#include <QColor>
#include <QKeyEvent>
#include <QMouseEvent>
#include <QPainter>
#include <QWidget>
#include <stdio.h>
#include <stdlib.h>
#include <vector>

using namespace std;

class Canvas : public QWidget {
    Q_OBJECT
public:
    typedef vector<double> point_3d;
    using matrix = std::vector<std::vector<double>>;
    explicit Canvas(QWidget* parent = nullptr);
    ~Canvas();
    void paintEvent(QPaintEvent* event);

    void clear();
    void set_bg(const QColor& color);
    void set_fg(const QColor& color);
    void rotate_trans_matrix(matrix rotate_matrix);
    void rotate_x(double angle);
    void rotate_y(double angle);
    void rotate_z(double angle);
    void set_scale(double sc);
    void trans_point(point_3d& point);
    bool is_visible(double x, double y);
    bool draw_point(double x, double y, vector<double>& hh, vector<double>& lh);
    void draw_horizon_part(point_3d p1, point_3d p2, vector<double>& hh, vector<double>& lh);
    void draw_horizon(double (*func)(double x, double z), vector<double>& hh, vector<double>& lh, double fr, double to, double step, double z);
    void draw(double (*f)(double x, double z), double x_min, double x_max, double x_step, double z_min, double z_max, double z_step);

private:
    QColor bg = QColor(255, 255, 255);
    QColor fg = QColor(0, 0, 0);
    QRect field;
    matrix trans_matrix = { { 1, 0, 0, 0 }, { 0, 1, 0, 0 }, { 0, 0, 1, 0 }, { 0, 0, 0, 1 } };
    double scale = 100;

    QImage image;
    QPainter painter;

signals:
};

#endif // CANVAS_H
