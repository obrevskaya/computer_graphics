#include "figures.h"
#include <QPen>
#include <cmath>

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

void Rectangle::paint(QPainter& painter) const
{
    painter.setPen(QPen(fg));
    painter.drawRect(p1.get_x(), p1.get_y(), p2.get_x() - p1.get_x(), p2.get_y() - p1.get_y());
}

Point Point::operator-(const Point& p)
{
    return Point(x - p.x, y - p.y);
}

Point Point::operator+(const Point& p)
{
    return Point(x + p.x, y + p.y);
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

Point Rectangle::get_p1() const
{
    return p1;
}

Point Rectangle::get_p2() const
{
    return p2;
}

void Rectangle::set_p1(const Point& p)
{
    p1 = p;
}

void Rectangle::set_p2(const Point& p)
{
    p2 = p;
}

int Rectangle::get_top() const
{
    if (p1.get_y() > p2.get_y())
        return p1.get_y();

    return p2.get_y();
}

int Rectangle::get_bottom() const
{
    if (p1.get_y() < p2.get_y())
        return p1.get_y();

    return p2.get_y();
}

int Rectangle::get_left() const
{
    if (p1.get_x() < p2.get_x())
        return p1.get_x();

    return p2.get_x();
}

int Rectangle::get_right() const
{
    if (p1.get_x() > p2.get_x())
        return p1.get_x();

    return p2.get_x();
}
