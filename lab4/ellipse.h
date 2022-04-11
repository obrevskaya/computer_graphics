#ifndef ELLIPSE_H
#define ELLIPSE_H

#include <QColor>
#include <QPointF>
#include "methods.h"



class Ellipse
{
public:
    double a;
    double b;
    QPointF center;
    double minX, maxX;
    double minY, maxY;
    method_t method;
    QColor color;

public:
    Ellipse(double _a, double _b, QPointF _center, method_t _method, const QColor &color);
};

#endif // ELLIPSE_H
