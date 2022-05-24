#include "figures.h"
#include <QPen>
#include <cmath>

#define EPS 1e-6

void Point::paint(QPainter& painter) const
{
    painter.setPen(QPen(fg));
    painter.drawPoint(x, y);
}

void Line::paint(QPainter& painter) const
{
    painter.setPen(QPen(fg));
    painter.drawLine(p1.get_x(), p1.get_y(), p2.get_x(), p2.get_y());
}

Point Line::distance(const Point& a)
{
    Point p = p1 - p2;
    Point h(0, 0);

    if (p.get_x() == 0) {
        h.set_x(p1.get_x());
        h.set_y(a.get_y());
    } else if (p.get_y() == 0) {
        h.set_x(a.get_x());
        h.set_y(p1.get_y());
    } else {
        double hy = ((p.get_x() * p.get_y()
                             * (a.get_x() - p1.get_x())
                         + p1.get_y() * p.get_x() * p.get_x() + p.get_y() * p.get_y() * a.get_y())
            / (p.get_x() * p.get_x() + p.get_y() * p.get_y()));
        h.set_x((hy - p1.get_y()) / p.get_y() * p.get_x() + p1.get_x());
        h.set_y(hy);
    }

    return h;
}

Point Point::operator-(const Point& p)
{
    return Point(x - p.x, y - p.y);
}

Point Point::operator+(const Point& p)
{
    return Point(x + p.x, y + p.y);
}

Point Point::operator*(double num)
{
    return Point(x * num, y * num);
}

Point Point::operator/(double num)
{
    return Point(x / num, y / num);
}

bool Point::operator==(const Point& other)
{
    return (this->get_x() == other.get_x() && this->get_y() == other.get_y());
}

bool Point::operator<(const Point& p)
{
    if (x < p.x && y <= p.y)
        return true;

    return false;
}

double Point::get_x() const
{
    return x;
}

double Point::get_y() const
{
    return y;
}

void Point::set_x(int x)
{
    this->x = x;
}

void Point::set_y(int y)
{
    this->y = y;
}

Point get_vector(const Point& p1, const Point& p2)
{
    return Point(p2.get_x() - p1.get_x(), p2.get_y() - p1.get_y());
}

static int vector_prod(const Point& v1, const Point& v2)
{
    return v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x();
}

Point Line::get_p1()
{
    return p1;
}

Point Line::get_p2()
{
    return p2;
}

void Line::set_p1(const Point& p)
{
    p1 = p;
}

void Line::set_p2(const Point& p)
{
    p2 = p;
}

void Cutter::paint(QPainter& painter) const
{
    painter.setPen(QPen(fg));
    for (auto edge : edges)
        painter.drawLine(edge.get_p1().get_x(), edge.get_p1().get_y(), edge.get_p2().get_x(), edge.get_p2().get_y());
}

Line Cutter::get_edge(size_t index)
{
    return edges[index];
}

int Cutter::sign(int num)
{
    if (num == 0)
        return 0;
    return (num > 0) ? 1 : -1;
}

static void get_d_k_b(int ax, int ay, int cx, int cy, double& d, double& k, double& b)
{
    // Коэффициенты прямой АС
    // Если точки A и С лежат на одной вертикальной прямой
    if (cx == ax) {
        k = 1;
        b = -cx;
        d = 0;
    } else {
        k = 1.0 * (cy - ay) / (cx - ax);
        b = cy - (k * cx);
        d = 1;
    }
}

static bool are_crossed(int ax, int ay, int bx, int by, int cx, int cy, int dx, int dy)
{
    double d_ab, k_ab, b_ab;
    double d_cd, k_cd, b_cd;
    get_d_k_b(ax, ay, bx, by, d_ab, k_ab, b_ab);
    get_d_k_b(cx, cy, dx, dy, d_cd, k_cd, b_cd);

    if (abs(k_ab - k_cd) < EPS)
        return false;
    double x = (b_cd - b_ab) / (k_ab - k_cd);
    double y;
    if (d_cd < EPS)
        y = (k_ab * x + b_ab);
    else if (d_ab < EPS)
        y = (k_cd * x + b_cd);
    else
        y = (k_ab * x + b_ab);
    int b1 = ax;
    int b2 = bx;
    ax = std::max(b1, b2);
    bx = std::min(b1, b2);
    b1 = ay;
    b2 = by;
    ay = std::max(b1, b2);
    by = std::min(b1, b2);

    if ((abs(bx - x) < EPS) || (abs(ax - x) < EPS) || (abs(by - y) < EPS) || (abs(ay - y) < EPS))
        return false;
    if (bx < x && x < ax && by < y && y < ay)
        return true;
    else
        return false;
}

bool Cutter::check_cross()
{
    bool f;
    for (size_t i = 0; i < points.size() - 1; i++)
        for (size_t j = i + 1; j < points.size(); j++) {
            if (j == points.size() - 1) {
                f = are_crossed(points[i].get_x(), points[i].get_y(), points[i + 1].get_x(), points[i + 1].get_y(),
                    points[j].get_x(), points[j].get_y(), points[0].get_x(), points[0].get_y());
                if (f)
                    return true;
            } else {
                f = are_crossed(points[i].get_x(), points[i].get_y(), points[i + 1].get_x(), points[i + 1].get_y(),
                    points[j].get_x(), points[j].get_y(), points[j + 1].get_x(), points[j + 1].get_y());
                if (f)
                    return true;
            }
        }
    return false;
}

int Cutter::is_convex()
{
    if (points.size() < 3)
        return 0;

    Point v1 = get_vector(points[points.size() - 1], points[0]);
    Point v2 = get_vector(points[points.size() - 2], points[points.size() - 1]);

    int prev = sign(vector_prod(v1, v2));

    for (size_t i = 1; i < points.size() - 2; i++) {
        v1 = get_vector(points[i - 1], points[i]);
        if (i == 1)
            v2 = get_vector(points[points.size() - 1], points[i - 1]);
        else
            v2 = get_vector(points[i - 2], points[i - 1]);

        int cur = sign(vector_prod(v1, v2));
        if (prev != cur)
            return 0;
        prev = cur;
    }

    if (check_cross())
        return 0;

    return prev;
}

void Cutter::add_point(Point p)
{
    points.push_back(p);
    if (points.size() > 1)
        edges.push_back(Line(points[points.size() - 1], points[points.size() - 2]));
}

void Cutter::change_last(Point p)
{
    points[points.size() - 1] = p;
    if (points.size() > 1)
        edges[edges.size() - 1].set_p1(p);
}

void Cutter::close()
{
    points.pop_back();
    if (points.size() > 1)
        edges[edges.size() - 1].set_p1(points[0]);
}
