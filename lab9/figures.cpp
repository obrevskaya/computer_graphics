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

bool Line::find_intersection(Point& p, Line& line, int norm)
{
    bool vis1 = is_visible(line.get_p1(), norm);
    bool vis2 = is_visible(line.get_p2(), norm);
    bool is_inter = (vis1 || vis2) && (!(vis1 && vis2));

    if (is_inter) {
        Point cut_vec = get_p2() - get_p1();
        Point pol_vec = line.get_p2() - line.get_p1();

        long nominator = cut_vec.get_y() * (line.get_p1().get_x() - get_p1().get_x()) - cut_vec.get_x() * (line.get_p1().get_y() - get_p1().get_y());
        long denominator = pol_vec.get_y() * cut_vec.get_x() - pol_vec.get_x() * cut_vec.get_y();

        if (denominator == 0) {
            p = line.get_p2();
        } else {
            double t = (double)nominator / denominator;
            p = line.get_p1() + (line.get_p2() - line.get_p1()) * t;
        }
    }

    return is_inter;
}

bool Line::is_visible(const Point& p, int norm)
{
    Point v1 = get_p2() - get_p1();
    Point v2 = Line(get_p2(), p).get_p2() - Line(get_p2(), p).get_p1();

    int pr = v1.get_x() * v2.get_y() - v1.get_y() * v2.get_x();

    if (pr * norm >= 0)
        return true;

    return false;
}

Point Line::find_distance(const Point& a)
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

void Polygon::paint(QPainter& painter) const
{
    painter.setPen(QPen(fg));
    for (auto edge : edges)
        painter.drawLine(edge.get_p1().get_x(), edge.get_p1().get_y(), edge.get_p2().get_x(), edge.get_p2().get_y());
}

Line Polygon::get_edge(size_t index)
{
    return edges[index];
}

int Polygon::sign(int num)
{
    if (num == 0)
        return 0;
    return (num > 0) ? 1 : -1;
}

int Polygon::is_convex()
{
    Point p1, p2, p3;
    int x1, y1, x2, y2, r, prev = 0, curr;
    for (size_t i = 0; i < points.size() - 2; i++) {
        p1 = points[i];
        p2 = points[i + 1];
        p3 = points[i + 2];

        x1 = p2.get_x() - p1.get_x();
        y1 = p2.get_y() - p1.get_y();

        x2 = p3.get_x() - p2.get_x();
        y2 = p3.get_y() - p2.get_y();

        r = x1 * y2 - x2 * y1;
        curr = sign(r);

        if (!prev)
            prev = curr;
        if (curr && curr != prev)
            return 0;
    }

    p1 = points[points.size() - 2];
    p2 = points[points.size() - 1];
    p3 = points[0];

    x1 = p2.get_x() - p1.get_x();
    y1 = p2.get_y() - p1.get_y();

    x2 = p3.get_x() - p2.get_x();
    y2 = p3.get_y() - p2.get_y();

    r = x1 * y2 - x2 * y1;
    curr = sign(r);
    if (curr && curr != prev)
        return 0;

    p1 = points[points.size() - 1];
    p2 = points[0];
    p3 = points[1];

    x1 = p2.get_x() - p1.get_x();
    y1 = p2.get_y() - p1.get_y();

    x2 = p3.get_x() - p2.get_x();
    y2 = p3.get_y() - p2.get_y();

    r = x1 * y2 - x2 * y1;
    curr = sign(r);
    if (curr && curr != prev)
        return 0;

    Point tmp;
    for (size_t i = 0; i < edges_count() - 1; i++)
        for (size_t j = i + 1; j < edges_count(); j++)
            if (edges[i].find_intersection(tmp, edges[j], prev))
                return 0;

    return prev;
}

void Polygon::add_point(Point p)
{
    points.push_back(p);
    if (points.size() > 1)
        edges.push_back(Line(points[points.size() - 2], points[points.size() - 1]));
}

void Polygon::change_last(Point p)
{
    points[points.size() - 1] = p;
    if (points.size() > 1)
        edges[edges.size() - 1].set_p2(p);
}

void Polygon::close()
{
    if (points.size() > 1)
        edges.push_back(Line(points[points.size() - 1], points[0]));
}

void Polygon::close_with_pop()
{
    points.pop_back();
    if (points.size() > 1)
        edges[edges.size() - 1].set_p2(points[0]);
}
