#include "ellipse.h"


Ellipse::Ellipse(double _a, double _b, QPointF _center, method_t _method = STANDART, const QColor &_color = Qt::white)
    : a(_a), b(_b), center(_center),
      maxX(_center.x() + _a), minX(_center.x() - _a),
      maxY(_center.y() + _b), minY(_center.y() - _b),
      method(_method),
      color(_color)
{
}
