#ifndef CIRCLE_H
#define CIRCLE_H

#include <QColor>
#include <QPointF>
#include "methods.h"


class Circle
{
public:
    double r;
    QPointF center;
    double minX, maxX;
    double minY, maxY;
    method_t method;
    QColor color;

public:
    Circle(double r, QPointF(center), method_t method, const QColor &color);
};

#endif // CIRCLE_H
